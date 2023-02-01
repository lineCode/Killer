#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Killer/Combat/HealthInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Killer/General/FunctionLibrary.h"
#include "HealthComponent.generated.h"

class ADamageNumbers;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KILLER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UWorld* World;

	AActor* Owner;
	IHealthInterface* HealthInterfaceOwner;

	float CurrentHealth;
	float MaxHealth;

	void SpawnNumbers(TSubclassOf<ADamageNumbers> NumbersClass, float Value);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float MaxMinHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float MaxMaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		TSubclassOf<ADamageNumbers> DamageNumbersClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		TSubclassOf<ADamageNumbers> HealNumbersClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float NumbersSpawnRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		USoundWave* DamageSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		UNiagaraSystem* BloodParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		TSubclassOf<UDamageType> ReturnToPlayerDamageType;

public:
	UHealthComponent();

	void Heal(float HealAmount);

	float GetHealthPercent();

	UFUNCTION()
		void OnActorTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
