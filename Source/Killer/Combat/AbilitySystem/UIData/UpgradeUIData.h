#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectUIData.h"
#include "UpgradeUIData.generated.h"

UCLASS()
class KILLER_API UUpgradeUIData : public UGameplayEffectUIData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Data")
	FText DisplayName;
};
