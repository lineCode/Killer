#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Killer/Combat/Health/HealthInterface.h"
#include "Killer/Combat/Projectiles/BulletInfo.h"
#include "MainCharacter.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UPaperFlipbook;
class AEffectsActor;
class UHealthComponent;
class AMainCharacterHUD;
class AMainCharacterController;
class UWeaponComponent;
class AGun;

UCLASS()
class KILLER_API AMainCharacter : public APaperCharacter, public IHealthInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UWeaponComponent* WeaponComponent;

	/** When player causes damage to others, he takes damage too. Damage multiplied by this value. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Self Damage")
	float SelfDamageMultiplier{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Self Damage")
	TSubclassOf<UDamageType> SelfDamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Animation")
	UPaperFlipbook* IdleFlipbook{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Animation")
	UPaperFlipbook* RunFlipbook{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Animation")
	UPaperFlipbook* JumpFlipbook{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Effects|Footsteps")
	UNiagaraSystem* WalkParticles{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Effects|Footsteps")
	float SpeedForWalkParticles{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Effects|Footsteps")
	float FootstepsSoundInterval{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Effects|Footsteps")
	USoundWave* FootstepsSound{};

	UPROPERTY(BlueprintReadWrite, Category = "Main Character|Effects|Footsteps")
	UNiagaraComponent* FootstepsParticlesComponent{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Effects|Landing")
	float LandingImpactSpeed{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Effects|Landing")
	TSubclassOf<AEffectsActor> LandingEffectsActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Effects|Landing")
	TSubclassOf<UCameraShakeBase> LandingCameraShakeClass;

	UPROPERTY(Replicated)
	AMainCharacterController* MainCharacterController{};

	UPROPERTY()
	AMainCharacterHUD* MainCharacterHUD{};

	UPROPERTY(ReplicatedUsing="OnRep_PlayerMaterial")
	UMaterialInterface* PlayerMaterial;

	UFUNCTION()
	void OnRep_PlayerMaterial();

	UFUNCTION(Server, Reliable)
	void Server_ChangePlayerMaterial(UMaterialInterface* Material);

	UFUNCTION(Client, Reliable)
	void Client_ChangePlayerMaterial();

	void TeleportPlayerToRandomSpawn();

	void UpdateCharacterAnimation() const;
	void RotateCharacter() const;

	void ActivateWalkParticles() const;

	FTimerHandle FootstepsTimerHandle;
	void PlayFootstepsSound() const;

	void InitializeFootstepsEffects();

	UFUNCTION(Server, Reliable)
	void Server_ChangePlayerName(const FString& Name);

public:
	AMainCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void Landed(const FHitResult& Hit) override;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FBulletInfo BulletModifiers{};

	virtual void OnKilled(AController* InstigatedBy, AActor* DamageCauser) override;
	virtual void OnRevived() override;

	virtual void OnDamageCaused(AActor* DamageCausedTo, float Damage) override;
	virtual void OnKillCaused(AActor* KillCausedTo) override;

	AMainCharacterController* GetMainCharacterController() const { return MainCharacterController; }
	AMainCharacterHUD* GetMainCharacterHUD() const { return MainCharacterHUD; }

	void SetMainCharacterController(AMainCharacterController* NewController)
	{
		MainCharacterController = NewController;
	}

	void SetMainCharacterHUD(AMainCharacterHUD* NewHUD) { MainCharacterHUD = NewHUD; }

	UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	UWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }

	UMaterialInterface* GetPlayerMaterial() const { return PlayerMaterial; } 

	UFUNCTION(Client, Reliable)
	void Client_OnKilled();

	UFUNCTION(Client, Reliable)
	void Client_OnRevived();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnRevived();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnKilled();
};
