#include "TimerWidget.h"
#include "TextWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"

void UTimerWidget::DisplayTime(const float Seconds) const
{
	TextWidget->SetDisplayText(UKismetTextLibrary::AsTimespan_Timespan(UKismetMathLibrary::FromSeconds(Seconds)));
}

void UTimerWidget::DecrementTime()
{
	if (TimeInSeconds <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		RemoveFromParent();

		return;
	}
	
	DisplayTime(--TimeInSeconds);
}

void UTimerWidget::StartTimer(const float Seconds)
{
	TimeInSeconds = Seconds;
	
	DisplayTime(Seconds);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTimerWidget::DecrementTime, 1.0f, true, 1.0f);
}
