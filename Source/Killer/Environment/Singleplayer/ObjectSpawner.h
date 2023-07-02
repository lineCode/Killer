#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectSpawner.generated.h"

class AObjectSpawn;

UCLASS()
class KILLER_API AObjectSpawner : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<AActor*> FreeSpawns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner")
	TSubclassOf<AObjectSpawn> SpawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner")
	int32 MinSpawnsCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner")
	int32 MaxSpawnsCount;

public:
	AObjectSpawner();

	void SpawnObjectAtRandomFreeSpawn(AActor* FreeSpawn);
};
