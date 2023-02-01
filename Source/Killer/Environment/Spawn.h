#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.h"
#include "Spawn.generated.h"

UCLASS()
class KILLER_API ASpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawn();

protected:
	ASpawner* ObjectSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> ActorClass;

public:
	virtual AActor* SpawnObject(ASpawner* Spawner);

	UFUNCTION()
		void OnObjectDestroyed(AActor* DestroyedActor);
};
