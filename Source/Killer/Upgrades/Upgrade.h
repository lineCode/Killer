#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Killer/Player/MainCharacter.h"
#include "Killer/Weapons/Gun.h"
#include "Upgrade.generated.h"

UCLASS()
class KILLER_API AUpgrade : public AActor
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    UWorld* World;

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UBoxComponent* BoxComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UPaperFlipbookComponent* FlipbookComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade|Effects")
    FEffectsInfo PickupEffectsInfo;

    /**
     * UpgradeLocation = Sin(TimeSinceGameStarted * AnimationHalfHeightMultiplier) * AnimationSpeed * DeltaSeconds;
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade|Animation")
    float AnimationHalfHeightMultiplier;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade|Animation")
    float AnimationSpeed;

    void AnimateUpgrade(float DeltaSeconds);

public:
    AUpgrade();

    virtual void Tick(float DeltaSeconds) override;

    virtual void Activate(AMainCharacter* MainCharacter);

    UFUNCTION()
    void OnUpgradeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent*
                               OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
