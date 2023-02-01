#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Killer/General/FunctionLibrary.h"
#include "Bullet.generated.h"

UCLASS()
class KILLER_API ABullet : public AActor
{
	GENERATED_BODY()
	
private:
	AController* Instigator;

protected:
	virtual void BeginPlay() override;

	UWorld* World;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float MinDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float MaxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
		UNiagaraSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
		USoundWave* HitSound;

public:
	ABullet();

	void FireInDirection(const FVector& Direction, AController* InstigatedBy);

	UFUNCTION()
		void OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnBulletOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
