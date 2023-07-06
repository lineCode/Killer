#include "MultiplayerSessionsMenuWidget.h"
#include "MultiplayerSessionWidget.h"
#include "Components/UniformGridPanel.h"

void UMultiplayerSessionsMenuWidget::DisplaySessions(TArray<FOnlineSessionSearchResult> SessionsSearchResults) const
{
	LoadingCircleWidget->SetVisibility(ESlateVisibility::Hidden);
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Display Sessions"));
	int32 RowsCount = 0;
	for (auto& Result : SessionsSearchResults)
	{
		if (SessionInfoWidgetClass)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("SessionWidgetClass"));
		}
		if (GetWorld())
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("World"));
		}
		if (auto* SessionWidget = CreateWidget<UMultiplayerSessionWidget>(GetWorld(), SessionInfoWidgetClass))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Widget Created"));
			SessionsGrid->AddChildToUniformGrid(SessionWidget, RowsCount++);
			
			SessionWidget->DisplaySessionInfo(Result);
		}
	}
}
