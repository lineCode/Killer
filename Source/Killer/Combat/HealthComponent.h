#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class AEffectsActor;
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

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component|Numbers")
	TSubclassOf<AHealthNumbers> DamageNumbersClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component|Numbers")
	TSubclassOf<AHealthNumbers> HealNumbersClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component|Numbers")
	float NumbersSpawnRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component|Effects")
	TSubclassOf<AEffectsActor> DamageEffectsActor;

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

	UPROPERTY(Replicated)
	AActor* Owner;

	UPROPERTY(Replicated)
	float MaxHealth;

	UPROPERTY(ReplicatedUsing="OnRep_CurrentHealth")
	float CurrentHealth;

	UFUNCTION()
	void OnRep_CurrentHealth();

	void SpawnHealthNumbers(TSubclassOf<AHealthNumbers> NumbersClass, float Value) const;

	void SpawnDamageEffects() const;

public:
	UHealthComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool IsDead() const { return bIsDead; }

	void InitializeOwnerDynamicMaterials();

	UFUNCTION(Server, Reliable)
	void Server_ReviveOwner();

	UFUNCTION(Server, Reliable)
	void Server_HealOwner(float HealAmount);

	UFUNCTION(Server, Reliable)
	void Server_SetCurrentHealth(float Value);

	UFUNCTION(Server, Reliable)
	void Server_DamageOwner(AController* InstigatedBy, AActor* DamageCauser, float Damage);

	UFUNCTION(Server, Reliable)
	void Server_KillOwner(AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnActorTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                          class AController* InstigatedBy, AActor* DamageCauser);
};
