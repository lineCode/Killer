#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Killer/Player/MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Killer/General/FunctionLibrary.h"
#include "Gun.generated.h"

class ABullet;

UCLASS()
class KILLER_API AGun : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* MuzzleLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ABullet> BulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundWave* ShootSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UNiagaraSystem* ShootParticles;

public:
	AGun();

	AMainCharacter* Owner;

	void FireFromMuzzle(AController* InstigatedBy);

	UPaperFlipbookComponent* GetSprite();
};
