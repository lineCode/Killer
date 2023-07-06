#include "MultiplayerSessionWidget.h"
#include "Killer/General/Save/SessionSubsystem.h"
#include "Killer/UI/Elements/ButtonWidget.h"
#include "Killer/UI/Elements/TextWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

void UMultiplayerSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ConnectButton->OnButtonClicked.AddDynamic(this, &UMultiplayerSessionWidget::OnConnectButtonClicked);
}

void UMultiplayerSessionWidget::OnConnectButtonClicked(UButtonWidget* Button)
{
	if (auto* SessionSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<USessionSubsystem>())
	{
		SessionSubsystem->JoinGameSession(Session);
	}
}

void UMultiplayerSessionWidget::DisplaySessionInfo(const FOnlineSessionSearchResult& SessionSearchResult)
{
	Session = SessionSearchResult;
	
	ServerNameText->SetDisplayText(FText::FromString(UKismetStringLibrary::GetSubstring(SessionSearchResult.Session.OwningUserName, 0, MaxServerNameLength)));

	const int32 MaxNumPlayers = SessionSearchResult.Session.SessionSettings.NumPublicConnections;
	const int32 NumPlayers = MaxNumPlayers - SessionSearchResult.Session.NumOpenPublicConnections;
	NumPlayersText->SetDisplayText(FText::FromString(FString::FromInt(NumPlayers) + "/" + FString::FromInt(MaxNumPlayers)));

	PingText->SetDisplayText(FText::FromString(FString::FromInt(SessionSearchResult.PingInMs)));
}