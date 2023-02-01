#include "Spawner.h"
#include "Kismet/GameplayStatics.h"
#include "Spawn.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	if (!World) return;

	UGameplayStatics::GetAllActorsOfClass(World, SpawnClass, FreeSpawns);

	MaxSpawnsCount = FMath::Clamp(MaxSpawnsCount, 0, FreeSpawns.Num());
	int32 SpawnsCount = FMath::RandRange(MinSpawnsCount, MaxSpawnsCount);

	TArray<ASpawn*> ValidSpawns;
	for (size_t i = 0; i < SpawnsCount; i++)
	{
		int32 RandomIndex = FMath::RandRange(0, FreeSpawns.Num() - 1);

		ASpawn* Spawn = Cast<ASpawn>(FreeSpawns[RandomIndex]);

		FreeSpawns.RemoveAt(RandomIndex);

		ValidSpawns.Add(Spawn);
	}

	for (auto& Spawn : ValidSpawns)
	{
		Spawn->SpawnObject(this);
	}
}

void ASpawner::SpawnTargetAtRandomFreeSpawn(AActor* FreeSpawn)
{
	int32 RandomIndex = FMath::RandRange(0, FreeSpawns.Num() - 1);

	ASpawn* Spawn = Cast<ASpawn>(FreeSpawns[RandomIndex]);
	if (!Spawn) return;

	Spawn->SpawnObject(this);

	FreeSpawns.RemoveAt(RandomIndex);

	if (!FreeSpawn) return;

	FreeSpawns.Add(FreeSpawn);
}