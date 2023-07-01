#include "HealUpgrade.h"

void AHealUpgrade::Activate(AMainCharacter* MainCharacter)
{
	Super::Activate(MainCharacter);

	if (!MainCharacter)
	{
		return;
	}

	UHealthComponent* HealthComponent = MainCharacter->GetHealthComponent();
	if (!HealthComponent)
	{
		return;
	}

	const float HealAmount = FMath::RandRange(MinHealAmount, MaxHealAmount);

	HealthComponent->Server_HealOwner(HealAmount);

	Destroy();
}
