#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "Killer/Combat/Health/HealthInterface.h"
#include "Target.generated.h"

class UHealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTargetKilled, AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS()
class KILLER_API ATarget : public APawn, public IHealthInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target|Patrol")
	float HalfPatrolDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target|Patrol")
	float AcceptanceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target|Patrol")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target|Patrol")
	bool IsPlacedInWorld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target|Effects")
	TSubclassOf<UCameraShakeBase> DamageCameraShakeClass;

	FVector FirstLocation;
	FVector SecondLocation;

	bool IsMovingToFirstLocation;

	void MoveToLocation(const FVector& Location);

	void SetLocations();

public:
	ATarget();

	UPROPERTY(BlueprintAssignable)
	FOnTargetKilled OnTargetKilled;

	void SetHalfPatrolDistance(float Value);

	virtual void Tick(float DeltaTime) override;

	virtual void OnDamageTaken(AController* InstigatedBy, AActor* DamageCauser) override;
	virtual void OnKilled(AController* InstigatedBy, AActor* DamageCauser) override;
};
