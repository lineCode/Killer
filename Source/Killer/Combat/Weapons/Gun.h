#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Gun.generated.h"

struct FProjectileInfo;
class AEffectsActor;
class AProjectile;

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
	TSubclassOf<AProjectile> BulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Shooting")
	float FireRate;

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

public:
	AGun();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool IsAutomatic() const { return bIsAutomatic; }

	UPaperFlipbookComponent* GetSprite() const { return FlipbookComponent; }

	float GetFireRate() const { return FireRate; }
	FTransform GetMuzzleTransform() const { return MuzzleLocation->GetComponentTransform(); }
	UMaterialInterface* GetGunMaterial() const { return GunMaterial; }
	TSubclassOf<AProjectile> GetProjectileClass() const { return BulletClass; }
	TSubclassOf<AEffectsActor> GetGunshotEffectsActorClass() const { return GunshotEffectsActorClass; }
	TSubclassOf<UCameraShakeBase> GetGunshotCameraShakeClass() const { return GunshotCameraShakeClass; }
};
