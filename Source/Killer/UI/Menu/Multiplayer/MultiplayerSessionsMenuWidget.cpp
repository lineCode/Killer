#include "MultiplayerSessionsMenuWidget.h"
#include "MultiplayerSessionWidget.h"
#include "Components/UniformGridPanel.h"

void UMultiplayerSessionsMenuWidget::DisplaySessions(TArray<FOnlineSessionSearchResult> SessionsSearchResults) const
{
	LoadingCircleWidget->SetVisibility(ESlateVisibility::Hidden);
	
	int32 RowsCount = 0;
	for (auto& Result : SessionsSearchResults)
	{
		if (auto* SessionWidget = CreateWidget<UMultiplayerSessionWidget>(GetOwningPlayer(), SessionInfoWidgetClass))
		{
			SessionsGrid->AddChildToUniformGrid(SessionWidget, RowsCount++);
			
			SessionWidget->DisplaySessionInfo(Result);
		}
	}
}
