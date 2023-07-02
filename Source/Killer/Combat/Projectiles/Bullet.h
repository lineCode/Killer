#pragma once

#include "CoreMinimal.h"
#include "BulletInfo.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Bullet.generated.h"

class AEffectsActor;

UCLASS()
class KILLER_API ABullet : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Info")
	FBulletInfo BulletInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Effects")
	TSubclassOf<AEffectsActor> DestroyEffectsActor;

	void SpawnBulletDestroyEffects() const;

public:
	ABullet();

	/** Needs to be called before bullet spawn finished. */
	void ModifyBulletInfo(const FBulletInfo& BulletModifiers);

	UFUNCTION()
	void OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                 FVector NormalImpulse, const FHitResult& Hit);
};
