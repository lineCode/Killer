#pragma once

#include "CoreMinimal.h"
#include "HealthInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UHealthInterface : public UInterface
{
    GENERATED_BODY()
};

class IHealthInterface
{
    GENERATED_BODY()

public:
    virtual void OnDamageTaken(AController* InstigatedBy, AActor* DamageCauser);
    virtual void OnKilled(AController* InstigatedBy, AActor* DamageCauser);

    virtual void OnHealed(float HealAmount);
    virtual void OnRevived();

    virtual void OnDamageCaused(AActor* DamageCausedTo, float Damage);
    virtual void OnKillCaused(AActor* KillCausedTo);
};
