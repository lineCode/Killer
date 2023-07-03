#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerWidget.generated.h"

class UTextWidget;

UCLASS()
class KILLER_API UTimerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextWidget* TextWidget;

	float TimeInSeconds;

	void DisplayTime(float Seconds) const;

	FTimerHandle TimerHandle;
	void DecrementTime();
	
public:
	void StartTimer(float Seconds);
};
