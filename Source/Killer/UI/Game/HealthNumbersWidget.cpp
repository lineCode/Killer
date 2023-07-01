#include "HealthNumbersWidget.h"

#include "Kismet/KismetTextLibrary.h"

void UHealthNumbersWidget::ShowHealthNumbers(const float Value, const FLinearColor& Color)
{
	HealthNumbersTextBlock->SetText(UKismetTextLibrary::Conv_DoubleToText(Value, HalfToEven, false,
	                                                                      true, 1, 324, 0, 2));

	FSlateFontInfo HealthNumbersFontInfo = HealthNumbersTextBlock->GetFont();
	HealthNumbersFontInfo.OutlineSettings.OutlineColor = Color;

	HealthNumbersTextBlock->SetFont(HealthNumbersFontInfo);

	PlayAnimation(HealthNumbersTextBlockAnimation);
}
