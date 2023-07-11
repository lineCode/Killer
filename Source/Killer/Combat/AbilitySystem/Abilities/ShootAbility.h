#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ShootAbility.generated.h"

class AGun;
class AEffectsActor;
class AProjectile;

UCLASS()
class KILLER_API UShootAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	void SpawnProjectile() const;

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SpawnGunshotEffects();

	void StartGunshotCameraShake() const;

	AGun* GetGun() const;
	FTransform GetProjectileSpawnTransform() const;

public:
	UShootAbility();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) const override;
};
