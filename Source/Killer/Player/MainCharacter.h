#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Killer/Combat/HealthComponent.h"
#include "Killer/Combat/HealthInterface.h"
#include "MainCharacter.generated.h"

class AGun;

UCLASS()
class KILLER_API AMainCharacter : public APaperCharacter, public IHealthInterface
{
	GENERATED_BODY()

private:
	APlayerController* PlayerController;
	AGun* Gun;

	UMaterialInstanceDynamic* PlayerDynamicMaterial;
	UMaterialInstanceDynamic* WeaponDynamicMaterial;
	float PlayerEmission;
	float WeaponEmission;

	void UpdateMaterialEmission(UMaterialInstanceDynamic* DynamicMaterial, float Emission);
	void GetMaterialEmission(UPaperFlipbookComponent* FlipbookSprite, UMaterialInstanceDynamic*& DynamicMaterial, float& Emission);

	void MoveWeapon();
	void RotateWeapon();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UChildActorComponent* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UHealthComponent* HealthComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		bool IsDead;

	UPROPERTY(BlueprintReadWrite)
		int32 Kills;

public:
	AMainCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void OnDamageTaken(AController* InstigatedBy, AActor* DamageCauser) override;
	virtual void OnKilled(AController* InstigatedBy, AActor* DamageCauser) override;
	virtual void OnHealed(float HealAmount) override;

	void AddKills(int32 Value);

	AGun* GetGun();
	UHealthComponent* GetHealthComponent();
	bool GetIsDead();

	UFUNCTION(BlueprintImplementableEvent)
		void _OnKilled(AController* InstigatedBy, AActor* DamageCauser);
};
