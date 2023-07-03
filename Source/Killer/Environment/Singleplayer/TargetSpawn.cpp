#include "TargetSpawn.h"
#include "Killer/General/GameModes/GameModeBaseSingleplayer.h"
#include "Killer/Targets/Target.h"

AActor* ATargetSpawn::SpawnRandomObject(AObjectSpawner* Spawner)
{
	AActor* SpawnedObject = Super::SpawnRandomObject(Spawner);
	if (!SpawnedObject)
	{
		return nullptr;
	}

	ATarget* Target = Cast<ATarget>(SpawnedObject);
	if (!Target)
	{
		return nullptr;
	}

	Target->SetHalfPatrolDistance(HalfPatrolDistance);

	Target->OnTargetKilled.AddDynamic(this, &ATargetSpawn::OnTargetKilled);

	return Target;
}

void ATargetSpawn::OnTargetKilled(AController* InstigatedBy, AActor* DamageCauser)
{
	if (KillerGameModeBase)
	{
		KillerGameModeBase->IncrementEnemiesKilled();
	}
}
