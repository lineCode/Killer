#include "PlayersPanelWidget.h"

void UPlayersPanelWidget::DisplayKillsCount(const int32 Value) const
{
	const FTextFormat FormattedText(FText::FromString("Kills: {Kills}"));
			
	FFormatNamedArguments NamedArguments;
	NamedArguments.Add(TEXT("Kills"), Value);
	
	KillsCountTextBox->SetText(FText::Format(FormattedText, NamedArguments));
}
