#include "ShootAbility.h"
#include "Killer/Combat/Projectiles/Projectile.h"
#include "Killer/Combat/Weapons/Gun.h"
#include "Killer/Combat/Weapons/WeaponComponent.h"
#include "Killer/Effects/EffectsActor.h"
#include "Killer/Player/General/MainCharacter.h"

UShootAbility::UShootAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UShootAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority)
	{
		SpawnProjectile();
		Multicast_SpawnGunshotEffects();
	}

	if (const APawn* OwnerPawn = Cast<APawn>(GetOwningActorFromActorInfo()))
	{
		if (OwnerPawn->IsLocallyControlled())
		{
			StartGunshotCameraShake();
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UShootAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (const UGameplayEffect* CooldownGE = GetCooldownGameplayEffect(); const AGun* Gun = GetGun())
	{
		const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Cooldown")), Gun->GetFireRate());
		
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}

void UShootAbility::SpawnProjectile() const
{
	FActorSpawnParameters ProjectileSpawnParameters;
	ProjectileSpawnParameters.Instigator = Cast<APawn>(GetOwningActorFromActorInfo());
	ProjectileSpawnParameters.Owner = GetOwningActorFromActorInfo();
	
	if (const AGun* Gun = GetGun())
	{
		GetWorld()->SpawnActor<AProjectile>(Gun->GetProjectileClass(), GetProjectileSpawnTransform(), ProjectileSpawnParameters);
	}
}

void UShootAbility::Multicast_SpawnGunshotEffects_Implementation()
{
	const AGun* Gun = GetGun();
	if (!Gun)
	{
		return;
	}

	const TSubclassOf<AEffectsActor> GunshotEffectsActorClass = Gun->GetGunshotEffectsActorClass();
	if (!GunshotEffectsActorClass)
	{
		return;
	}
	
	if (auto* GunshotEffectsActor = GetWorld()->SpawnActor<AEffectsActor>(GunshotEffectsActorClass, GetProjectileSpawnTransform()))
	{
		GunshotEffectsActor->Server_SetParticlesMaterial(Gun->GetGunMaterial());
	}
}

void UShootAbility::StartGunshotCameraShake() const
{
	const AGun* Gun = GetGun();
	if (!Gun)
	{
		return;
	}

	const TSubclassOf<UCameraShakeBase> GunshotCameraShakeClass = Gun->GetGunshotCameraShakeClass();
	if (!GunshotCameraShakeClass)
	{
		return;
	}

	const APawn* OwnerPawn = Cast<APawn>(GetOwningActorFromActorInfo());
	if (!OwnerPawn)
	{
		return;
	}

	AController* OwnerController = OwnerPawn->GetController();
	if (!OwnerController)
	{
		return;
	}
	
	if (const APlayerController* PlayerController = Cast<APlayerController>(OwnerController))
	{
		PlayerController->PlayerCameraManager->StartCameraShake(GunshotCameraShakeClass);
	}
}

AGun* UShootAbility::GetGun() const
{
	const AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetOwningActorFromActorInfo());
	if (!MainCharacter)
	{
		return {};
	}

	const UWeaponComponent* WeaponComponent = MainCharacter->GetWeaponComponent();
	if (!WeaponComponent)
	{
		return {};
	}

	return WeaponComponent->GetGun();
}

FTransform UShootAbility::GetProjectileSpawnTransform() const
{
	const AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetOwningActorFromActorInfo());
	if (!MainCharacter)
	{
		return {};
	}

	const UWeaponComponent* WeaponComponent = MainCharacter->GetWeaponComponent();
	if (!WeaponComponent)
	{
		return {};
	}

	const AGun* Gun = WeaponComponent->GetGun();
	if (!Gun)
	{
		return {};
	}

	return Gun->GetMuzzleTransform();
}