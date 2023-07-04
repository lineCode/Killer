#pragma once

#include "CoreMinimal.h"
#include "RestartTextWidget.h"
#include "RestartTextWidgetMultiplayer.generated.h"

UCLASS()
class KILLER_API URestartTextWidgetMultiplayer : public URestartTextWidget
{
	GENERATED_BODY()

public:
	virtual void ShowDeathText() override;
};
