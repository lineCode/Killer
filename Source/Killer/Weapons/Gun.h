#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Killer/Combat/BulletInfo.h"
#include "Killer/General/FunctionLibrary.h"
#include "Gun.generated.h"

class ABullet;

UCLASS()
class KILLER_API AGun : public AActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
    UBoxComponent* BoxComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
    UPaperFlipbookComponent* FlipbookComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
    USceneComponent* MuzzleLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Shooting")
    TSubclassOf<ABullet> BulletClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Shooting")
    float TimeToShoot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Shooting")
    bool bIsAutomatic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Shooting|Effects")
    FEffectsInfo GunshotEffectsInfo;

    bool bCanShoot;

    FTimerHandle FireRateTimerHandle;
    void ResetFireRate();

    void SpawnBullet(const FBulletInfo& BulletModifiers);
    void SpawnEffects();

public:
    AGun();

    bool IsAutomatic() const { return bIsAutomatic; }

    void FireFromMuzzle(const FBulletInfo& BulletModifiers);

    UPaperFlipbookComponent* GetSprite() const { return FlipbookComponent; }
};
