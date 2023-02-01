#include "Spawn.h"

ASpawn::ASpawn()
{
	PrimaryActorTick.bCanEverTick = false;

}

AActor* ASpawn::SpawnObject(ASpawner* Spawner)
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	if (Spawner)
	{
		ObjectSpawner = Spawner;
	}

	AActor* SpawnedActor = World->SpawnActor<AActor>(ActorClass, GetActorTransform());
	if (SpawnedActor)
	{
		SpawnedActor->OnDestroyed.AddDynamic(this, &ASpawn::OnObjectDestroyed);
	}

	return SpawnedActor;
}

void ASpawn::OnObjectDestroyed(AActor* DestroyedActor)
{
	if (ObjectSpawner)
	{
		ObjectSpawner->SpawnTargetAtRandomFreeSpawn(this);
	}
}

