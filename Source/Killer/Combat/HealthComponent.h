#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Killer/Combat/HealthInterface.h"
#include "Killer/General/FunctionLibrary.h"
#include "HealthComponent.generated.h"

class AHealthNumbers;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KILLER_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component|Health")
    float MaxMinHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component|Health")
    float MaxMaxHealth;

    UPROPERTY(BlueprintReadWrite)
    bool bIsDead;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component|Numbers")
    TSubclassOf<AHealthNumbers> DamageNumbersClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component|Numbers")
    TSubclassOf<AHealthNumbers> HealNumbersClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component|Numbers")
    float NumbersSpawnRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component|Effects")
    FEffectsInfo DamageEffectsInfo;
    
    /**
     * When HP changes, owner's and its children flipbooks change their emission in percentages.
     * Flipbooks should have material with scalar "Emission" parameter.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health Component|Effects")
    bool bShouldChangeMaterialEmission;

    /** Material - Emission. */
    UPROPERTY()
    TMap<UMaterialInstanceDynamic*, float> OwnerDynamicMaterials;

    TMap<UMaterialInstanceDynamic*, float> GetAllDynamicMaterialsFromActor(const AActor* Actor) const;
	
    void MultiplyDynamicMaterialsEmissions(TMap<UMaterialInstanceDynamic*, float> DynamicMaterials, float Value) const;

    float MaxHealth;

    UPROPERTY()
    float CurrentHealth;

    UPROPERTY()
    UWorld* World;

    UPROPERTY()
    AActor* Owner;
    
    IHealthInterface* HealthInterfaceOwner;

    void ShowHealthNumbers(TSubclassOf<AHealthNumbers> NumbersClass, float Value) const;

public:
    UHealthComponent();

    void Heal(float HealAmount);

    float GetHealthPercent() const { return CurrentHealth / MaxHealth; }
    
    bool IsDead() const { return bIsDead; }

    void InitializeOwnerDynamicMaterials();

    void SetCurrentHealth(float Value);

    void DamageOwner(AController* InstigatedBy, AActor* DamageCauser, float Damage);
    void KillOwner(AController* InstigatedBy, AActor* DamageCauser);
    
    UFUNCTION()
    void OnActorTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                              class AController* InstigatedBy, AActor* DamageCauser);
};
