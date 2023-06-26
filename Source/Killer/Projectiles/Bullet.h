#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Killer/Combat/BulletInfo.h"
#include "Killer/General/FunctionLibrary.h"
#include "Bullet.generated.h"

class AEffectsActor;

UCLASS()
class KILLER_API ABullet : public AActor
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    UWorld* World;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
    UBoxComponent* BoxComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
    UPaperFlipbookComponent* FlipbookComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
    UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Damage")
    float MinDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Damage")
    float MaxDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Damage")
    TSubclassOf<UDamageType> DamageTypeClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Effects")
    TSubclassOf<AEffectsActor> DestroyEffectsActor;

    float Damage;

    FBulletInfo BulletInfoModifiers;

    UFUNCTION(Server, Unreliable)
    void Server_SpawnBulletDestroyEffects();

public:
    ABullet();

    /** Needs to be called before bullet spawn finished. */
    void ModifyBulletInfo(const FBulletInfo& BulletModifiers);

    UFUNCTION()
    void OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                     FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    void OnBulletOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult);
};
