#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Killer/Combat/HealthComponent.h"
#include "Killer/Combat/HealthInterface.h"
#include "Components/AudioComponent.h"
#include "Killer/Combat/BulletInfo.h"
#include "Killer/Targets/TargetEventsInterface.h"
#include "MainCharacter.generated.h"

class AGun;

UCLASS()
class KILLER_API AMainCharacter : public APaperCharacter, public IHealthInterface, public ITargetEventsInterface
{
	GENERATED_BODY()

private:
	UWorld* World;

	AGun* Gun;

	APlayerController* PlayerController;

	UMaterialInstanceDynamic* PlayerDynamicMaterial;
	UMaterialInstanceDynamic* WeaponDynamicMaterial;
	float PlayerEmission;
	float WeaponEmission;

	void MoveWeapon();
	void RotateWeapon();

	void UpdateMaterialEmission(UMaterialInstanceDynamic* DynamicMaterial, float Emission);
	void GetMaterialEmission(UPaperFlipbookComponent* FlipbookSprite, UMaterialInstanceDynamic*& DynamicMaterial, float& Emission);

	void TeleportPlayerToRandomSpawn();

	void LoadFromSave();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAudioComponent* AudioComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UHealthComponent* HealthComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		bool IsDead;

	UPROPERTY(BlueprintReadWrite)
		int32 Kills;

public:
	AMainCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBulletInfo BulletModifiers;

	virtual void Tick(float DeltaTime) override;

	virtual void OnDamageTaken(AController* InstigatedBy, AActor* DamageCauser) override;
	virtual void OnKilled(AController* InstigatedBy, AActor* DamageCauser) override;
	virtual void OnHealed(float HealAmount) override;

	virtual void OnTargetKilled(AController* InstigatedBy, AActor* DamageCauser) override;

	AGun* GetGun();
	UHealthComponent* GetHealthComponent();
	bool GetIsDead();

	UFUNCTION(BlueprintImplementableEvent)
		void _OnKilled(AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(NetMulticast, Reliable)
		void SpawnWeaponMulticast();

	UFUNCTION(Server, Unreliable)
		void MoveWeaponMulticast(const FVector& Location);

	UFUNCTION(Server, Unreliable)
		void RotateWeaponMulticast(const FQuat& Rotation);
};
