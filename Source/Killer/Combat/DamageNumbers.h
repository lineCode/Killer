#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "DamageNumbers.generated.h"

UCLASS()
class KILLER_API ADamageNumbers : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UWidgetComponent* WidgetComponent;

public:
	ADamageNumbers();

	UFUNCTION(BlueprintImplementableEvent)
		void SpawnDamageNumbers(float Damage);
};
