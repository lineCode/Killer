#include "ImpulseForceUpgrade.h"
#include "ActiveGameplayEffectHandle.h"

void AImpulseForceUpgrade::ActivateUpgrade(AMainCharacter* MainCharacter)
{
	Super::ActivateUpgrade(MainCharacter);

	ApplyGameplayEffectToMainCharacter(MainCharacter, ImpulseForceMultiplierEffectClass, this);

	Destroy();
}
