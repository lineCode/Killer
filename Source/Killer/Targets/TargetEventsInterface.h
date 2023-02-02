#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TargetEventsInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UTargetEventsInterface : public UInterface
{
    GENERATED_BODY()
};

class ITargetEventsInterface
{
    GENERATED_BODY()

public:
    virtual void OnTargetKilled(AController* InstigatedBy, AActor* DamageCauser);
};
