#include "UpgradeSpawnMultiplayer.h"
#include "Killer/Upgrades/Upgrade.h"

AUpgradeSpawnMultiplayer::AUpgradeSpawnMultiplayer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUpgradeSpawnMultiplayer::OnUpgradeDestroyed(AActor* DestroyedActor)
{
	bIsSpawnTaken = false;
}

bool AUpgradeSpawnMultiplayer::TrySpawnUpgrade(const TSubclassOf<AUpgrade> UpgradeClass)
{
	if (bIsSpawnTaken || !UpgradeClass)
	{
		return false;
	}

	if (auto* Upgrade = GetWorld()->SpawnActor<AUpgrade>(UpgradeClass, GetActorTransform()))
	{
		Upgrade->OnDestroyed.AddDynamic(this, &AUpgradeSpawnMultiplayer::OnUpgradeDestroyed);

		bIsSpawnTaken = true;

		return true;
	}
	
	return false;
}

