#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class ASpawn;

UCLASS()
class KILLER_API ASpawner : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	TArray<AActor*> FreeSpawns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ASpawn> SpawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MinSpawnsCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxSpawnsCount;

public:
	ASpawner();

	void SpawnTargetAtRandomFreeSpawn(AActor* FreeSpawn);
};
