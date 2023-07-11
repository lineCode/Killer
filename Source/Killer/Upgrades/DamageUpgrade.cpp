#include "DamageUpgrade.h"
#include "ActiveGameplayEffectHandle.h"

void ADamageUpgrade::ActivateUpgrade(AMainCharacter* MainCharacter)
{
	Super::ActivateUpgrade(MainCharacter);

	ApplyGameplayEffectToMainCharacter(MainCharacter, DamageMultiplierEffectClass, this);

	Destroy();
}
