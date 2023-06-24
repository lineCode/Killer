#pragma once

#include "CoreMinimal.h"
#include "ObjectSpawn.h"
#include "TargetSpawn.generated.h"

UCLASS()
class KILLER_API ATargetSpawn : public AObjectSpawn
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target")
    float HalfPatrolDistance;

    UFUNCTION()
    void OnTargetKilled(AController* InstigatedBy, AActor* DamageCauser);
    
public:
    virtual AActor* SpawnRandomObject(AObjectSpawner* Spawner) override;
};
