#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Killer/Player/MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Killer/General/FunctionLibrary.h"
#include "Killer/Combat/BulletInfo.h"
#include "Killer/Combat/ParticlesAndSound.h"
#include "Gun.generated.h"

class ABullet;

UCLASS()
class KILLER_API AGun : public AActor
{
	GENERATED_BODY()

private:
	float CurrentTimeToShoot;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* MuzzleLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ABullet> BulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TimeToShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AParticlesAndSound> GunshotEffects;

public:
	AGun();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsAutomatic;

	AMainCharacter* Owner;

	void FireFromMuzzle(const FBulletInfo& BulletModifiers);

	UPaperFlipbookComponent* GetSprite();
};
