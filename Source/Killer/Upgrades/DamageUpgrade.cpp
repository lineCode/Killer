#include "DamageUpgrade.h"

void ADamageUpgrade::Activate(AMainCharacter* MainCharacter)
{
    Super::Activate(MainCharacter);
    
    if (!MainCharacter) return;

    MainCharacter->BulletModifiers.DamageMultiplier *= DamageMultiplier;

    Destroy();
}
