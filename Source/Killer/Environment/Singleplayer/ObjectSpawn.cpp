#include "ObjectSpawn.h"
#include "Killer/General/GameModes/GameModeBaseSingleplayer.h"
#include "Kismet/GameplayStatics.h"


AObjectSpawn::AObjectSpawn()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AObjectSpawn::BeginPlay()
{
	Super::BeginPlay();

	if (AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(GetWorld()))
	{
		KillerGameModeBase = Cast<AGameModeBaseSingleplayer>(GameModeBase);
	}
}

AActor* AObjectSpawn::SpawnRandomObject(AObjectSpawner* Spawner)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return {};
	}

	if (Spawner)
	{
		ObjectSpawner = Spawner;
	}

	const TSubclassOf<AActor> SpawnedActorClass = GetRandomObjectToSpawn(GetSpawnableObjects());
	if (!SpawnedActorClass)
	{
		return {};
	}

	AActor* SpawnedActor = World->SpawnActor<AActor>(SpawnedActorClass, GetActorTransform());
	if (SpawnedActor)
	{
		SpawnedActor->OnDestroyed.AddDynamic(this, &AObjectSpawn::OnObjectDestroyed);
	}

	return SpawnedActor;
}

void AObjectSpawn::OnObjectDestroyed(AActor* DestroyedActor)
{
	if (ObjectSpawner)
	{
		ObjectSpawner->SpawnObjectAtRandomFreeSpawn(this);
	}
}

TArray<FObjectToSpawn> AObjectSpawn::GetSpawnableObjects()
{
	if (!KillerGameModeBase)
	{
		return {};
	}

	TArray<FObjectToSpawn> SpawnableActors;
	for (auto& Object : ObjectsToSpawn)
	{
		if (Object.KillsToUnlock <= KillerGameModeBase->GetEnemiesKilled())
		{
			SpawnableActors.Add(Object);
		}
	}

	return {};
}

TSubclassOf<AActor> AObjectSpawn::GetRandomObjectToSpawn(TArray<FObjectToSpawn> ObjectToSpawn)
{
	float Bound = 0.0f;
	float ChancesSum = 0.0f;

	for (const auto& Object : ObjectToSpawn)
	{
		ChancesSum += Object.ChanceToSpawn;
	}

	const float RandomFloat = FMath::RandRange(0.0f, ChancesSum);

	for (const auto& Object : ObjectsToSpawn)
	{
		if (Object.ChanceToSpawn >= Bound && RandomFloat <= Bound + Object.ChanceToSpawn)
		{
			return Object.ObjectClassToSpawn;
		}

		Bound += Object.ChanceToSpawn;
	}

	return {};
}
