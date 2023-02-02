#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "Killer/Combat/HealthInterface.h"
#include "Killer/Combat/HealthComponent.h"
#include "Killer/Targets/TargetEventsInterface.h"
#include "Target.generated.h"

UCLASS()
class KILLER_API ATarget : public APawn, public IHealthInterface
{
	GENERATED_BODY()

private:
	FVector FirstLocation;
	FVector SecondLocation;

	bool IsMovingToFirstLocation;

	void MoveToLocation(const FVector& Location);

	void SetLocations();

protected:
	virtual void BeginPlay() override;

	UWorld* World;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patroul")
		float HalfPatroulDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patroul")
		float AcceptanceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patroul")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patroul")
		bool IsPlacedInWorld;

public:	
	ATarget();

	void SetHalfPatroulDistance(float Value);

	virtual void Tick(float DeltaTime) override;

	virtual void OnKilled(AController* InstigatedBy, AActor* DamageCauser) override;
};
