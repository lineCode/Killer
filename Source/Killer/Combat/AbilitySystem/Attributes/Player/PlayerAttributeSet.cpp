#include "PlayerAttributeSet.h"
#include "Net/UnrealNetwork.h"

UPlayerAttributeSet::UPlayerAttributeSet()
{
}

void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Health, COND_None, REPNOTIFY_Always);
}

void UPlayerAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Health, OldHealth);
}

void UPlayerAttributeSet::OnRep_ProjectileDamage(const FGameplayAttributeData& OldProjectileDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, ProjectileDamage, OldProjectileDamage);
}

void UPlayerAttributeSet::OnRep_ProjectileDamageModifier(const FGameplayAttributeData& OldProjectileDamageModifier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, ProjectileDamageModifier, OldProjectileDamageModifier);
}

void UPlayerAttributeSet::OnRep_ProjectileImpulseForce(const FGameplayAttributeData& OldProjectileImpulseForce)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, ProjectileImpulseForce, OldProjectileImpulseForce);
}
