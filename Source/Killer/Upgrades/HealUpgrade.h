#pragma once

#include "CoreMinimal.h"
#include "Upgrade.h"
#include "HealUpgrade.generated.h"

UCLASS()
class KILLER_API AHealUpgrade : public AUpgrade
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heal Upgrade")
	float MinHealAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heal Upgrade")
	float MaxHealAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heal Upgrade")
	TSubclassOf<UGameplayEffect> HealGameplayEffectClass;

public:
	virtual void ActivateUpgrade(AMainCharacter* MainCharacter) override;
};
