#include "HealthComponent.h"

#include "PaperFlipbookComponent.h"
#include "Killer/Combat/HealthNumbers.h"
#include "Killer/Player/MainCharacter.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    World = GetWorld();
    Owner = GetOwner();
    
    if (Owner)
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnActorTakeAnyDamage);

        HealthInterfaceOwner = Cast<IHealthInterface>(Owner);
    }

    if (bShouldChangeMaterialEmission)
    {
        InitializeOwnerDynamicMaterials();
    }

    MaxHealth = FMath::RandRange(MaxMinHealth, MaxMaxHealth);
    SetCurrentHealth(MaxHealth);
}

void UHealthComponent::OnActorTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                            AController* InstigatedBy, AActor* DamageCauser)
{
    Damage = FMath::Clamp(Damage, 0.0f, CurrentHealth);

    SetCurrentHealth(CurrentHealth - Damage);

    ShowHealthNumbers(DamageNumbersClass, Damage);

    if (Owner)
    {
        DamageEffectsInfo.Location = Owner->GetActorLocation();
        DamageEffectsInfo.Rotation = Owner->GetActorRotation();
        
        UFunctionLibrary::ActivateEffects(this, DamageEffectsInfo);
    }

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

void UHealthComponent::SetCurrentHealth(const float Value)
{
    CurrentHealth = FMath::Clamp(Value, 0.0f, MaxHealth);
    
    if (bShouldChangeMaterialEmission)
    {
        MultiplyDynamicMaterialsEmissions(OwnerDynamicMaterials, CurrentHealth / MaxHealth);
    }
}

void UHealthComponent::DamageOwner(AController* InstigatedBy, AActor* DamageCauser, const float Damage)
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

void UHealthComponent::KillOwner(AController* InstigatedBy, AActor* DamageCauser)
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

void UHealthComponent::ShowHealthNumbers(const TSubclassOf<AHealthNumbers> NumbersClass, const float Value) const
{
    if (!NumbersClass)
    {
        return;
    }
    
    FVector NumbersLocation = Owner->GetActorLocation();

    const float RandomAngle = FMath::RandRange(-180.0f, 180.0f);
    NumbersLocation.X += NumbersSpawnRadius * FMath::Cos(RandomAngle);
    NumbersLocation.Y += NumbersSpawnRadius * FMath::Sin(RandomAngle);

    if (const AHealthNumbers* HealthNumbers = World->SpawnActor<AHealthNumbers>(NumbersClass, NumbersLocation,
                                                                                Owner->GetActorRotation()))
    {
        HealthNumbers->ShowHealthNumbers(Value);
    }
}
