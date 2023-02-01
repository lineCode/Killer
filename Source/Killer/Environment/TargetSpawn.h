#pragma once

#include "CoreMinimal.h"
#include "Spawn.h"
#include "TargetSpawn.generated.h"

UCLASS()
class KILLER_API ATargetSpawn : public ASpawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HalfPatroulDistance;
	
public:
	virtual AActor* SpawnObject(ASpawner* Spawner) override;
};
