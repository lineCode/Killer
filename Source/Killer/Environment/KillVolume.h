#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "KillVolume.generated.h"

UCLASS()
class KILLER_API AKillVolume : public AActor
{
    GENERATED_BODY()

public:
    AKillVolume();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
    UBoxComponent* BoxComponent;

public:
    UFUNCTION()
    void OnKillVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult);
};
