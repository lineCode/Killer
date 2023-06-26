#include "HealthComponent.h"
#include "PaperFlipbookComponent.h"
#include "Killer/Effects/HealthNumbers.h"
#include "Killer/Effects/EffectsActor.h"
#include "Killer/Player/MainCharacter.h"
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
    DOREPLIFETIME_CONDITION(UHealthComponent, Owner, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UHealthComponent, HealthInterfaceOwner, COND_OwnerOnly);
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetOwnerRole() == ROLE_Authority)
    {
        Owner = GetOwner();
    
        if (Owner)
        {
            Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnActorTakeAnyDamage);

            HealthInterfaceOwner.SetInterface(Cast<IHealthInterface>(Owner));
        }

        MaxHealth = FMath::RandRange(MaxMinHealth, MaxMaxHealth);
        
        SetCurrentHealth(MaxHealth);
    }

    if (bShouldChangeMaterialEmission)
    {
        //InitializeOwnerDynamicMaterials();
    }
}

void UHealthComponent::OnActorTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                            AController* InstigatedBy, AActor* DamageCauser)
{
    Damage = FMath::Clamp(Damage, 0.0f, CurrentHealth);

    SetCurrentHealth(CurrentHealth - Damage);

    ShowHealthNumbers(DamageNumbersClass, Damage);

    Server_SpawnDamageEffects();

    DamageOwner(InstigatedBy, DamageCauser, Damage);
}

void UHealthComponent::Heal(float HealAmount)
{
    if (HealAmount <= 0.0f) return;

    const float OldCurrentHealth = CurrentHealth;
    SetCurrentHealth(CurrentHealth + HealAmount);
    
    HealAmount = CurrentHealth - OldCurrentHealth;

    if (HealNumbersClass)
    {
        ShowHealthNumbers(HealNumbersClass, HealAmount);
    }

    if (HealthInterfaceOwner)
    {
        HealthInterfaceOwner->OnHealed(HealAmount);
    }
}

void UHealthComponent::InitializeOwnerDynamicMaterials()
{
    OwnerDynamicMaterials.Empty();
	
    OwnerDynamicMaterials.Append(GetAllDynamicMaterialsFromActor(Owner));
	
    TArray<AActor*> AttachedActors;
    Owner->GetAttachedActors(AttachedActors);
    
    for (const auto& Actor : AttachedActors)
    {
        OwnerDynamicMaterials.Append(GetAllDynamicMaterialsFromActor(Actor));
    }
}

void UHealthComponent::SetCurrentHealth_Implementation(const float Value)
{
    CurrentHealth = FMath::Clamp(Value, 0.0f, MaxHealth);
    
    if (bShouldChangeMaterialEmission)
    {
        //MultiplyDynamicMaterialsEmissions(OwnerDynamicMaterials, CurrentHealth / MaxHealth);
    }
}

void UHealthComponent::DamageOwner_Implementation(AController* InstigatedBy, AActor* DamageCauser, const float Damage)
{
    if (HealthInterfaceOwner)
    {
        HealthInterfaceOwner->OnDamageTaken(InstigatedBy, DamageCauser);

        IHealthInterface* DamageCauserInterface = nullptr;
        if (InstigatedBy)
        {
            DamageCauserInterface = Cast<IHealthInterface>(InstigatedBy->GetPawn());
        }

        if (DamageCauserInterface)
        {
            DamageCauserInterface->OnDamageCaused(Owner, Damage);
        }

        if (CurrentHealth <= 0.0f)
        {
            KillOwner(InstigatedBy, DamageCauser);
        }
    }
}

void UHealthComponent::KillOwner_Implementation(AController* InstigatedBy, AActor* DamageCauser)
{
    bIsDead = true;

    IHealthInterface* KillCauserInterface = nullptr;
    if (InstigatedBy)
    {
        KillCauserInterface = Cast<IHealthInterface>(InstigatedBy->GetPawn());
    }

    if (KillCauserInterface)
    {
        KillCauserInterface->OnKillCaused(Owner);
    }
        
    HealthInterfaceOwner->OnKilled(InstigatedBy, DamageCauser);
}

TMap<UMaterialInstanceDynamic*, float> UHealthComponent::GetAllDynamicMaterialsFromActor(const AActor* Actor) const
{
    if (!Actor)
    {
        return TMap<UMaterialInstanceDynamic*, float>();
    }
	
    TMap<UMaterialInstanceDynamic*, float> DynamicMaterials;
	
    TSet<UActorComponent*> OwnerComponents = Actor->GetComponents();
    for (const auto& Component : OwnerComponents)
    {
        if (UPaperFlipbookComponent* PaperFlipbookComponent = Cast<UPaperFlipbookComponent>(Component))
        {
            for (int32 i = 0; i < PaperFlipbookComponent->GetNumMaterials(); ++i)
            {
                UMaterialInstanceDynamic* DynamicMaterial = PaperFlipbookComponent->CreateDynamicMaterialInstance(i);

                const FMaterialParameterInfo MaterialInfo = DynamicMaterial->GetParameterInfo(GlobalParameter, "Emission", nullptr);

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
    float Value) const
{
    for (const auto& Material : DynamicMaterials)
    {
        Material.Key->SetScalarParameterValue("Emission", Material.Value * Value);
    }
}

void UHealthComponent::ShowHealthNumbers_Implementation(const TSubclassOf<AHealthNumbers> NumbersClass, const float Value) const
{
    if (!NumbersClass)
    {
        return;
    }
    
    FVector NumbersLocation = Owner->GetActorLocation();

    const float RandomAngle = FMath::RandRange(-180.0f, 180.0f);
    NumbersLocation.X += NumbersSpawnRadius * FMath::Cos(RandomAngle);
    NumbersLocation.Y += NumbersSpawnRadius * FMath::Sin(RandomAngle);

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }
    
    if (const AHealthNumbers* HealthNumbers = World->SpawnActor<AHealthNumbers>(NumbersClass, NumbersLocation,
                                                                                Owner->GetActorRotation()))
    {
        HealthNumbers->ShowHealthNumbers(Value);
    }
}

void UHealthComponent::Server_SpawnDamageEffects_Implementation()
{
    if (UWorld* World = GetWorld(); DamageEffectsActor && Owner)
    {
        World->SpawnActor<AEffectsActor>(DamageEffectsActor, Owner->GetActorLocation(), Owner->GetActorRotation());
    }
}