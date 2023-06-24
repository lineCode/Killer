#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectSpawner.h"
#include "ObjectSpawn.generated.h"

class AKillerGameModeBase;

USTRUCT(BlueprintType)
struct FObjectToSpawn
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ObjectClassToSpawn;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 KillsToUnlock;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin = 0.0f, UIMax = 1.0f))
    float ChanceToSpawn;
};

UCLASS()
class KILLER_API AObjectSpawn : public AActor
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    
    UPROPERTY()
    AObjectSpawner* ObjectSpawner;

    UPROPERTY()
    AKillerGameModeBase* KillerGameModeBase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
    TArray<FObjectToSpawn> ObjectsToSpawn;

    TArray<FObjectToSpawn> GetSpawnableObjects();
    TSubclassOf<AActor> GetRandomObjectToSpawn(TArray<FObjectToSpawn> ObjectToSpawn);

public:
    AObjectSpawn();

    virtual AActor* SpawnRandomObject(AObjectSpawner* Spawner);

    UFUNCTION()
    void OnObjectDestroyed(AActor* DestroyedActor);
};
