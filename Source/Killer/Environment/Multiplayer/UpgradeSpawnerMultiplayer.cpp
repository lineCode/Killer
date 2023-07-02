#include "UpgradeSpawnerMultiplayer.h"
#include "UpgradeSpawnMultiplayer.h"
#include "Killer/Upgrades/Upgrade.h"
#include "Kismet/GameplayStatics.h"

AUpgradeSpawnerMultiplayer::AUpgradeSpawnerMultiplayer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUpgradeSpawnerMultiplayer::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GatherSpawnsAndStartUpgradeSpawnTimer();
	}
}

void AUpgradeSpawnerMultiplayer::SpawnRandomUpgradeAtRandomSpawn()
{
	if (AllFreeUpgradeSpawns.Num() <= 0)
	{
		GatherSpawnsAndStartUpgradeSpawnTimer();

		return;
	}
	
	const TSubclassOf<AUpgrade> RandomUpgradeClass = UpgradesClasses[FMath::RandRange(0, UpgradesClasses.Num() - 1)];

	const int32 RandomFreeUpgradeSpawnIndex = FMath::RandRange(0, AllFreeUpgradeSpawns.Num() - 1);
	AActor* RandomFreeUpgradeSpawn = AllFreeUpgradeSpawns[RandomFreeUpgradeSpawnIndex];
	
	if (auto* UpgradeSpawn = Cast<AUpgradeSpawnMultiplayer>(RandomFreeUpgradeSpawn))
	{
		if (!UpgradeSpawn->TrySpawnUpgrade(RandomUpgradeClass))
		{
			AllFreeUpgradeSpawns.RemoveAt(RandomFreeUpgradeSpawnIndex);
			
			SpawnRandomUpgradeAtRandomSpawn();

			return;
		}
	}
	
	GatherSpawnsAndStartUpgradeSpawnTimer();
}

void AUpgradeSpawnerMultiplayer::GatherSpawnsAndStartUpgradeSpawnTimer()
{
	UGameplayStatics::GetAllActorsOfClass(this, AUpgradeSpawnMultiplayer::StaticClass(), AllFreeUpgradeSpawns);
	
	const float TimeToSpawnUpgrade = FMath::RandRange(MinTimeToSpawnUpgrade, MaxTimeToSpawnUpgrade);
	
	GetWorldTimerManager().SetTimer(UpgradeSpawnTimerHandle, this,
		&AUpgradeSpawnerMultiplayer::SpawnRandomUpgradeAtRandomSpawn, TimeToSpawnUpgrade, false, TimeToSpawnUpgrade);
}

