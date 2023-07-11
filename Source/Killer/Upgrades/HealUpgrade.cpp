#include "HealUpgrade.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"

void AHealUpgrade::ActivateUpgrade(AMainCharacter* MainCharacter)
{
	Super::ActivateUpgrade(MainCharacter);

	if (!MainCharacter || !HealGameplayEffectClass)
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = MainCharacter->GetAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	EffectContext.AddInstigator(nullptr, this);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(HealGameplayEffectClass, 1, EffectContext);
	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Heal")),
			FMath::RandRange(MinHealAmount, MaxHealAmount));
		
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}

	Destroy();
}
