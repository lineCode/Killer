#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "HealthNumbers.generated.h"

UCLASS()
class KILLER_API AHealthNumbers : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UWidgetComponent* NumbersWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parameters")
	FLinearColor Color;

public:
	AHealthNumbers();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	float HealthValue;
};
