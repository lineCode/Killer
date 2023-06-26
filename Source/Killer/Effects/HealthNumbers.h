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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
    UWidgetComponent* NumbersWidgetComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parameters")
    FLinearColor Color;

public:
    AHealthNumbers();

    UFUNCTION(NetMulticast, Unreliable)
    void ShowHealthNumbers(float Value) const;
};
