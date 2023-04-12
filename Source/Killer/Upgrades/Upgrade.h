#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Killer/Player/MainCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Killer/Weapons/Gun.h"
#include "Killer/General/FunctionLibrary.h"
#include "Upgrade.generated.h"

UCLASS()
class KILLER_API AUpgrade : public AActor
{
	GENERATED_BODY()
	
protected:
	UWorld* World;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		TSubclassOf<AParticlesAndSound> PickupEffects;

public:
	AUpgrade();

	virtual void Activate(AMainCharacter* MainCharacter);

	UFUNCTION()
		void OnUpgradeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent*
			OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
