#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.h"
#include "Killer/Targets/TargetEventsInterface.h"
#include "Spawn.generated.h"

UCLASS()
class KILLER_API ASpawn : public AActor, public ITargetEventsInterface
{
	GENERATED_BODY()
	
private:
	int32 EnemiesKilled;

protected:
	ASpawner* ObjectSpawner;

	TSubclassOf<AActor> GetRandomSpawnableObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<int32, TSubclassOf<AActor>> EnemiesKilledToSpawnObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ChanceToSpawnFirstObject;

public:
	ASpawn();

	virtual AActor* SpawnObject(ASpawner* Spawner);

	virtual void OnTargetKilled(AController* InstigatedBy, AActor* DamageCauser) override;

	UFUNCTION()
		void OnObjectDestroyed(AActor* DestroyedActor);
};
