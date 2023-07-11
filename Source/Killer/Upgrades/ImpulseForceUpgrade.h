#pragma once

#include "CoreMinimal.h"
#include "Upgrade.h"
#include "ImpulseForceUpgrade.generated.h"

UCLASS()
class KILLER_API AImpulseForceUpgrade : public AUpgrade
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Impulse Force Upgrade")
	TSubclassOf<UGameplayEffect> ImpulseForceMultiplierEffectClass;

public:
	virtual void ActivateUpgrade(AMainCharacter* MainCharacter) override;
};
