#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Killer/Combat/HealthComponent.h"
#include "Killer/Combat/HealthInterface.h"
#include "Killer/Combat/BulletInfo.h"
#include "Killer/Projectiles/Bullet.h"
#include "MainCharacter.generated.h"

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
    float SelfDamageMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Self Damage")
    TSubclassOf<UDamageType> SelfDamageTypeClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Animation")
    UPaperFlipbook* IdleFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Animation")
    UPaperFlipbook* RunFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Animation")
    UPaperFlipbook* JumpFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Effects|Footsteps")
    UNiagaraSystem* WalkParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Effects|Footsteps")
    float SpeedForWalkParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Effects|Footsteps")
    float FootstepsSoundInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Effects|Footsteps")
    USoundWave* FootstepsSound;

    UPROPERTY(BlueprintReadWrite, Category = "Main Character|Effects|Footsteps")
    UNiagaraComponent* WalkParticlesComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Effects|Landing")
    float LandingImpactSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Character|Effects|Landing")
    FEffectsInfo LandingEffectsInfo;

    UPROPERTY(BlueprintReadWrite)
    int32 Kills;

    UPROPERTY()
    AMainCharacterController* MainCharacterController;

    UPROPERTY()
    AMainCharacterHUD* MainCharacterHUD;

    void TeleportPlayerToRandomSpawn();

    void UpdateCharacterAnimation() const;
    void RotateCharacter() const;

    void ActivateWalkParticles();

    FTimerHandle FootstepsTimerHandle;
    void PlayFootstepsSound();

    void InitializeFootstepsEffects();

public:
    virtual void Tick(float DeltaSeconds) override;
    
    virtual void PossessedBy(AController* NewController) override;

    void Landed(const FHitResult& Hit) override;
    
    AMainCharacter();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FBulletInfo BulletModifiers;

    virtual void OnKilled(AController* InstigatedBy, AActor* DamageCauser) override;

    virtual void OnDamageCaused(AActor* DamageCausedTo, float Damage) override;
    virtual void OnKillCaused(AActor* KillCausedTo) override;
    
    UHealthComponent* GetHealthComponent() const { return HealthComponent; }
    UWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }

    int32 GetKills() const { return Kills; }
};
