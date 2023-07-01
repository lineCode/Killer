#include "TextWidget.h"

void UTextWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetDisplayText(DisplayText);
}

void UTextWidget::SetDisplayText(const FText& TextToDisplay)
{
	DisplayText = TextToDisplay;

	if (TextBlock)
	{
		TextBlock->SetText(DisplayText);
	}
}
