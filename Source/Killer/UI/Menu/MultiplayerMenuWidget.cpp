#include "MultiplayerMenuWidget.h"
#include "MultiplayerSessionsMenuWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Killer/General/Save/SessionSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Killer/UI/Elements/ButtonWidget.h"
#include "Kismet/GameplayStatics.h"

void UMultiplayerMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SessionSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<USessionSubsystem>();
	if (SessionSubsystem)
	{
		SessionSubsystem->OnFindSessionsCompleteEvent.AddUObject(this, &UMultiplayerMenuWidget::OnFindSessionsComplete);
	}

	HostButton->OnButtonClicked.AddDynamic(this, &UMultiplayerMenuWidget::OnHostButtonClicked);
	FindSessionsButton->OnButtonClicked.AddDynamic(this, &UMultiplayerMenuWidget::OnFindSessionsButtonClicked);
	DirectConnectButton->OnButtonClicked.AddDynamic(this, &UMultiplayerMenuWidget::OnDirectConnectionButtonClicked);
}

void UMultiplayerMenuWidget::OnHostButtonClicked(UButtonWidget* Button)
{
	if (SessionSubsystem)
	{
		SessionSubsystem->CreateSession(4, true); // TODO: Move settings to UI.
	}
}

void UMultiplayerMenuWidget::OnFindSessionsButtonClicked(UButtonWidget* Button)
{
	if (SessionSubsystem)
	{
		SessionSubsystem->FindSessions(10, true); // TODO: Move settings to UI.
	}
	
	WidgetSwitcher->SetActiveWidget(SessionsMenuWidget);
}

void UMultiplayerMenuWidget::OnDirectConnectionButtonClicked(UButtonWidget* Button)
{
	
}

void UMultiplayerMenuWidget::OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SessionResults,
	bool Successful)
{
	SessionsMenuWidget->DisplaySessions(SessionResults);
}