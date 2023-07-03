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

	/** True - activates on overlap begin, false - activates on overlap end. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kill Volume")
	bool bReverseKillVolume;

	/** True - kills, false - teleports to random player start. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kill Volume")
	bool bShouldKill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kill Volume")
	TArray<TSubclassOf<AActor>> AffectedClasses;

	void TeleportActorToRandomPlayerStart(AActor* Actor) const;
	void KillActor(const AActor* Actor);

	void TryHandleVolumeOverlap(AActor* Actor);

public:
	UFUNCTION()
	void OnKillVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                              const FHitResult& SweepResult);

	UFUNCTION()
	void OnKillVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
