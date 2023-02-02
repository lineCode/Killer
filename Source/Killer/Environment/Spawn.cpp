#include "Spawn.h"

ASpawn::ASpawn()
{
	PrimaryActorTick.bCanEverTick = false;

}

TSubclassOf<AActor> ASpawn::GetRandomSpawnableObject()
{
	if (ChanceToSpawnFirstObject > 0.0f && FMath::RandRange(0.0f, 1.0f) <= ChanceToSpawnFirstObject)
	{
		return EnemiesKilledToSpawnObject[0];
	}

	TArray<TSubclassOf<AActor>> SpawnableActors;
	for (auto& Elem : EnemiesKilledToSpawnObject)
	{
		if (Elem.Key <= EnemiesKilled)
		{
			SpawnableActors.Add(Elem.Value);
		}
	}

	if (SpawnableActors.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, SpawnableActors.Num() - 1);
		return SpawnableActors[RandomIndex];
	}
	else
	{
		return TSubclassOf<AActor>();
	}

}

AActor* ASpawn::SpawnObject(ASpawner* Spawner)
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	if (Spawner)
	{
		ObjectSpawner = Spawner;
	}

	AActor* SpawnedActor = World->SpawnActor<AActor>(GetRandomSpawnableObject(), GetActorTransform());
	if (SpawnedActor)
	{
		SpawnedActor->OnDestroyed.AddDynamic(this, &ASpawn::OnObjectDestroyed);
	}

	return SpawnedActor;
}

void ASpawn::OnTargetKilled(AController* InstigatedBy, AActor* DamageCauser)
{
	EnemiesKilled++;
}

void ASpawn::OnObjectDestroyed(AActor* DestroyedActor)
{
	if (ObjectSpawner)
	{
		ObjectSpawner->SpawnObjectAtRandomFreeSpawn(this);
	}
}

