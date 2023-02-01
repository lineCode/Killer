#include "HealUpgrade.h"

void AHealUpgrade::Activate(AMainCharacter* MainCharacter)
{
	if (!MainCharacter) return;

	UHealthComponent* HealthComponent = MainCharacter->GetHealthComponent();
	if (!HealthComponent) return;

	float HealAmount = FMath::RandRange(MinHealAmount, MaxHealAmount);

	HealthComponent->Heal(HealAmount);

	Super::Activate(MainCharacter);

	Destroy();
}
