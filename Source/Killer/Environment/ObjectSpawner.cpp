#include "ObjectSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectSpawn.h"

AObjectSpawner::AObjectSpawner()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AObjectSpawner::BeginPlay()
{
    Super::BeginPlay();

    const UWorld* World = GetWorld();
    if (!World) return;

    UGameplayStatics::GetAllActorsOfClass(World, SpawnClass, FreeSpawns);

    MaxSpawnsCount = FMath::Clamp(MaxSpawnsCount, 0, FreeSpawns.Num());
    const int32 SpawnsCount = FMath::RandRange(MinSpawnsCount, MaxSpawnsCount);

    TArray<AObjectSpawn*> ValidSpawns;
    for (int32 i = 0; i < SpawnsCount; i++)
    {
        const int32 RandomIndex = FMath::RandRange(0, FreeSpawns.Num() - 1);

        AObjectSpawn* Spawn = Cast<AObjectSpawn>(FreeSpawns[RandomIndex]);

        FreeSpawns.RemoveAt(RandomIndex);

        ValidSpawns.Add(Spawn);
    }

    for (const auto& Spawn : ValidSpawns)
    {
        Spawn->SpawnRandomObject(this);
    }
}

void AObjectSpawner::SpawnObjectAtRandomFreeSpawn(AActor* FreeSpawn)
{
    const int32 RandomIndex = FMath::RandRange(0, FreeSpawns.Num() - 1);

    AObjectSpawn* Spawn = Cast<AObjectSpawn>(FreeSpawns[RandomIndex]);
    if (!Spawn) return;

    Spawn->SpawnRandomObject(this);

    FreeSpawns.RemoveAt(RandomIndex);

    if (!FreeSpawn) return;

    FreeSpawns.Add(FreeSpawn);
}
