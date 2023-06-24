#pragma once

#include "CoreMinimal.h"
#include "Upgrade.h"
#include "DamageUpgrade.generated.h"

UCLASS()
class KILLER_API ADamageUpgrade : public AUpgrade
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage Upgrade")
    float DamageMultiplier;

public:
    virtual void Activate(AMainCharacter* MainCharacter) override;
};
