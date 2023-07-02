#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UpgradeSpawnMultiplayer.generated.h"

class AUpgrade;

UCLASS()
class KILLER_API AUpgradeSpawnMultiplayer : public AActor
{
	GENERATED_BODY()

public:
	AUpgradeSpawnMultiplayer();

protected:
	bool bIsSpawnTaken;

	UFUNCTION()
	void OnUpgradeDestroyed(AActor* DestroyedActor);

public:
	bool TrySpawnUpgrade(TSubclassOf<AUpgrade> UpgradeClass);
};
