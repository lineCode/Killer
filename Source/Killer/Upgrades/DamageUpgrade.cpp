#include "DamageUpgrade.h"

void ADamageUpgrade::Activate(AMainCharacter* MainCharacter)
{
	if (!MainCharacter) return;

	MainCharacter->BulletModifiers.DamageMultiplier *= DamageMultiplier;

	Super::Activate(MainCharacter);

	Destroy();
}
