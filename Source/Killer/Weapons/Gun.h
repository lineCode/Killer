#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Killer/Combat/BulletInfo.h"
#include "Gun.generated.h"

class AEffectsActor;
class ABullet;

UCLASS()
class KILLER_API AGun : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
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
	TSubclassOf<AEffectsActor> GunshotEffectsActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Shooting|Effects")
	TSubclassOf<UCameraShakeBase> GunshotCameraShakeClass;

	UPROPERTY(ReplicatedUsing="OnRep_GunMaterial")
	UMaterialInterface* GunMaterial;

	UFUNCTION()
	void OnRep_GunMaterial();

	UFUNCTION(Server, Reliable)
	void Server_SetGunMaterial(UMaterialInterface* Material);

	bool bCanShoot;

	FTimerHandle FireRateTimerHandle;
	void ResetFireRate();

	UFUNCTION(Server, Reliable)
	void Server_SpawnBullet(const FBulletInfo& BulletModifiers);

	UFUNCTION(Server, Unreliable)
	void Server_SpawnGunshotEffects();

	void StartGunshotCameraShake() const;

public:
	AGun();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool IsAutomatic() const { return bIsAutomatic; }

	void FireFromMuzzle(const FBulletInfo& BulletModifiers);

	UPaperFlipbookComponent* GetSprite() const { return FlipbookComponent; }
};
