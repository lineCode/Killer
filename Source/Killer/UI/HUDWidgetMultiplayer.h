#pragma once

#include "CoreMinimal.h"
#include "HUDWidget.h"
#include "HUDWidgetMultiplayer.generated.h"

UCLASS()
class KILLER_API UHUDWidgetMultiplayer : public UHUDWidget
{
	GENERATED_BODY()

public:
	virtual void ShowDeathText() override;
};
