#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UpgradeSpawnerMultiplayer.generated.h"

class AUpgrade;

UCLASS()
class KILLER_API AUpgradeSpawnerMultiplayer : public AActor
{
	GENERATED_BODY()

public:
	AUpgradeSpawnerMultiplayer();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade Spawner")
	float MinTimeToSpawnUpgrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade Spawner")
	float MaxTimeToSpawnUpgrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade Spawner")
	TArray<TSubclassOf<AUpgrade>> UpgradesClasses;

	FTimerHandle UpgradeSpawnTimerHandle;
	void SpawnRandomUpgradeAtRandomSpawn();

	void GatherSpawnsAndStartUpgradeSpawnTimer();

	UPROPERTY()
	TArray<AActor*> AllFreeUpgradeSpawns;
};
