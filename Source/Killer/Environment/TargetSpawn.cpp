#include "TargetSpawn.h"
#include "Killer/Targets/Target.h"

AActor* ATargetSpawn::SpawnObject(ASpawner* Spawner)
{
	ATarget* Target = Cast<ATarget>(Super::SpawnObject(Spawner));
	if (!Target) return nullptr;

	Target->SetHalfPatroulDistance(HalfPatroulDistance);

	return Target;
}
