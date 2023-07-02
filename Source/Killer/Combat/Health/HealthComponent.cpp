#include "HealthComponent.h"
#include "HealthInterface.h"
#include "PaperFlipbookComponent.h"
#include "Killer/Effects/HealthNumbers.h"
#include "Killer/Effects/EffectsActor.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
	DOREPLIFETIME(UHealthComponent, MaxHealth);
	DOREPLIFETIME(UHealthComponent, bIsDead);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnActorTakeAnyDamage);

		MaxHealth = FMath::RandRange(MaxMinHealth, MaxMaxHealth);

		Server_SetCurrentHealth(MaxHealth);
	}
}

void UHealthComponent::OnActorTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                            AController* InstigatedBy, AActor* DamageCauser)
{
	Damage = FMath::Clamp(Damage, 0.0f, CurrentHealth);

	Server_SetCurrentHealth(CurrentHealth - Damage);

	SpawnHealthNumbers(DamageNumbersClass, Damage);

	SpawnDamageEffects();

	Server_DamageOwner(InstigatedBy, DamageCauser, Damage);
}

void UHealthComponent::Server_ReviveOwner_Implementation()
{
	bIsDead = false;

	Server_SetCurrentHealth(MaxHealth);

	if (auto* HealthInterfaceOwner = Cast<IHealthInterface>(GetOwner()))
	{
		HealthInterfaceOwner->OnRevived();
	}
}

void UHealthComponent::Server_HealOwner_Implementation(float HealAmount)
{
	if (HealAmount <= 0.0f)
	{
		return;
	}

	const float OldCurrentHealth = CurrentHealth;
	Server_SetCurrentHealth(CurrentHealth + HealAmount);

	HealAmount = CurrentHealth - OldCurrentHealth;

	if (HealNumbersClass)
	{
		SpawnHealthNumbers(HealNumbersClass, HealAmount);
	}

	if (auto* HealthInterfaceOwner = Cast<IHealthInterface>(GetOwner()))
	{
		HealthInterfaceOwner->OnHealed(HealAmount);
	}
}

void UHealthComponent::InitializeOwnerDynamicMaterials()
{
	OwnerDynamicMaterials.Empty();

	OwnerDynamicMaterials.Append(GetAllDynamicMaterialsFromActor(GetOwner()));

	TArray<AActor*> AttachedActors;
	GetOwner()->GetAttachedActors(AttachedActors);

	for (const auto& Actor : AttachedActors)
	{
		OwnerDynamicMaterials.Append(GetAllDynamicMaterialsFromActor(Actor));
	}
}

void UHealthComponent::TryInitializeOwnerDynamicMaterials()
{
	if (bShouldChangeMaterialEmission)
	{
		InitializeOwnerDynamicMaterials();
	}
}

void UHealthComponent::Server_SetCurrentHealth_Implementation(const float Value)
{
	CurrentHealth = FMath::Clamp(Value, 0.0f, MaxHealth);

	OnRep_CurrentHealth();
}

void UHealthComponent::Server_DamageOwner_Implementation(AController* InstigatedBy, AActor* DamageCauser,
                                                         const float Damage)
{
	if (auto* HealthInterfaceOwner = Cast<IHealthInterface>(GetOwner()))
	{
		HealthInterfaceOwner->OnDamageTaken(InstigatedBy, DamageCauser);

		if (InstigatedBy && InstigatedBy->GetPawn())
		{
			if (auto* DamageCauserInterface = Cast<IHealthInterface>(InstigatedBy->GetPawn()))
			{
				DamageCauserInterface->OnDamageCaused(GetOwner(), Damage);
			}
		}

		if (CurrentHealth <= 0.0f)
		{
			Server_KillOwner(InstigatedBy, DamageCauser);
		}
	}
}

void UHealthComponent::Server_KillOwner_Implementation(AController* InstigatedBy, AActor* DamageCauser)
{
	if (bIsDead)
	{
		return;
	}

	Server_SetCurrentHealth(0.0f);

	bIsDead = true;

	if (InstigatedBy && InstigatedBy->GetPawn())
	{
		if (auto* DamageCauserInterface = Cast<IHealthInterface>(InstigatedBy->GetPawn()))
		{
			DamageCauserInterface->OnKillCaused(GetOwner());
		}
	}

	if (auto* HealthInterfaceOwner = Cast<IHealthInterface>(GetOwner()))
	{
		HealthInterfaceOwner->OnKilled(InstigatedBy, DamageCauser);
	}
}

TMap<UMaterialInstanceDynamic*, float> UHealthComponent::GetAllDynamicMaterialsFromActor(const AActor* Actor) const
{
	if (!Actor)
	{
		return {};
	}

	TMap<UMaterialInstanceDynamic*, float> DynamicMaterials;

	TSet<UActorComponent*> OwnerComponents = Actor->GetComponents();
	for (const auto& Component : OwnerComponents)
	{
		if (auto* PaperFlipbookComponent = Cast<UPaperFlipbookComponent>(Component))
		{
			for (int32 i = 0; i < PaperFlipbookComponent->GetNumMaterials(); ++i)
			{
				UMaterialInstanceDynamic* DynamicMaterial = PaperFlipbookComponent->CreateDynamicMaterialInstance(i);

				const FMaterialParameterInfo MaterialInfo = DynamicMaterial->GetParameterInfo(
					GlobalParameter, "Emission", nullptr);

				if (float Emission; DynamicMaterial->GetScalarParameterValue(MaterialInfo, Emission))
				{
					DynamicMaterials.Add(DynamicMaterial, Emission);
				}
			}
		}
	}

	return DynamicMaterials;
}

void UHealthComponent::MultiplyDynamicMaterialsEmissions(TMap<UMaterialInstanceDynamic*, float> DynamicMaterials,
                                                         const float Value) const
{
	for (const auto& Material : DynamicMaterials)
	{
		Material.Key->SetScalarParameterValue("Emission", Material.Value * Value);
	}
}

void UHealthComponent::OnRep_CurrentHealth()
{
	if (bShouldChangeMaterialEmission)
	{
		MultiplyDynamicMaterialsEmissions(OwnerDynamicMaterials, CurrentHealth / MaxHealth);
	}
}

void UHealthComponent::SpawnHealthNumbers(const TSubclassOf<AHealthNumbers> NumbersClass, const float Value) const
{
	UWorld* World = GetWorld();

	if (!NumbersClass || !World)
	{
		return;
	}

	FTransform NumbersTransform;

	FVector NumbersLocation = GetOwner()->GetActorLocation();

	const float RandomAngle = FMath::RandRange(-180.0f, 180.0f);
	NumbersLocation.X += NumbersSpawnRadius * FMath::Cos(RandomAngle);
	NumbersLocation.Y += NumbersSpawnRadius * FMath::Sin(RandomAngle);

	NumbersTransform.SetLocation(NumbersLocation);
	NumbersTransform.SetRotation(GetOwner()->GetActorRotation().Quaternion());

	if (auto* HealthNumbers = World->SpawnActorDeferred<AHealthNumbers>(NumbersClass, NumbersTransform, GetOwner(),
	                                                                    Cast<APawn>(GetOwner())))
	{
		HealthNumbers->HealthValue = Value;

		UGameplayStatics::FinishSpawningActor(HealthNumbers, NumbersTransform);
	}
}

void UHealthComponent::SpawnDamageEffects() const
{
	if (UWorld* World = GetWorld(); DamageEffectsActor)
	{
		World->SpawnActor<AEffectsActor>(DamageEffectsActor, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	}
}
