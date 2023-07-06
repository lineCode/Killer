#include "SessionSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"

USessionSubsystem::USessionSubsystem()
: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionCompleted)),
  UpdateSessionCompleteDelegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnUpdateSessionCompleted)),
  StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionCompleted)),
  EndSessionCompleteDelegate(FOnEndSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnEndSessionCompleted)),
  DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionCompleted)),
  FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsCompleted)),
  JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionCompleted))
  
{
	
}

void USessionSubsystem::CreateSession(int32 NumPublicConnections, bool IsLANMatch)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnCreateSessionCompleteEvent.Broadcast(false);
		return;
	}

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->NumPrivateConnections = 0;
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowInvites = true;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
	LastSessionSettings->bIsDedicated = false;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bIsLANMatch = IsLANMatch;
	LastSessionSettings->bShouldAdvertise = true;

	LastSessionSettings->Set(SETTING_MAPNAME, FString("MultiplayerLobby"), EOnlineDataAdvertisementType::ViaOnlineService);

	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		OnCreateSessionCompleteEvent.Broadcast(false);
	}
}

void USessionSubsystem::OnCreateSessionCompleted(FName SessionName, bool Successful)
{
	if (const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld()))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		GetWorld()->ServerTravel("/Game/Levels/MultiplayerLobby?listen");
	}

	OnCreateSessionCompleteEvent.Broadcast(Successful);
}

void USessionSubsystem::UpdateSession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnUpdateSessionCompleteEvent.Broadcast(false);
		return;
	}

	const TSharedPtr<FOnlineSessionSettings> UpdatedSessionSettings = MakeShareable(new FOnlineSessionSettings(*LastSessionSettings));
	UpdatedSessionSettings->Set(SETTING_MAPNAME, FString("MultiplayerLobby"), EOnlineDataAdvertisementType::ViaOnlineService);

	UpdateSessionCompleteDelegateHandle =
		SessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegate);

	if (!SessionInterface->UpdateSession(NAME_GameSession, *UpdatedSessionSettings))
	{
		SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);

		OnUpdateSessionCompleteEvent.Broadcast(false);
	}
	else
	{
		LastSessionSettings = UpdatedSessionSettings;
	}
}

void USessionSubsystem::OnUpdateSessionCompleted(FName SessionName, bool Successful)
{
	if (const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld()))
	{
		SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);
	}

	OnUpdateSessionCompleteEvent.Broadcast(Successful);
}

void USessionSubsystem::StartSession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnStartSessionCompleteEvent.Broadcast(false);
		return;
	}

	StartSessionCompleteDelegateHandle =
		SessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);

	if (!SessionInterface->StartSession(NAME_GameSession))
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);

		OnStartSessionCompleteEvent.Broadcast(false);
	}
}

void USessionSubsystem::OnStartSessionCompleted(FName SessionName, bool Successful)
{
	if (const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld()))
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
	}

	OnStartSessionCompleteEvent.Broadcast(Successful);
}

void USessionSubsystem::EndSession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnEndSessionCompleteEvent.Broadcast(false);
		return;
	}

	EndSessionCompleteDelegateHandle =
		SessionInterface->AddOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegate);

	if (!SessionInterface->EndSession(NAME_GameSession))
	{
		SessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);

		OnEndSessionCompleteEvent.Broadcast(false);
	}
}

void USessionSubsystem::OnEndSessionCompleted(FName SessionName, bool Successful)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());;
	if (SessionInterface)
	{
		SessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);
	}

	OnEndSessionCompleteEvent.Broadcast(Successful);
}

void USessionSubsystem::DestroySession()
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnDestroySessionCompleteEvent.Broadcast(false);
		return;
	}

	DestroySessionCompleteDelegateHandle =
		SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	if (!SessionInterface->DestroySession(NAME_GameSession))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);

		OnDestroySessionCompleteEvent.Broadcast(false);
	}
}

void USessionSubsystem::OnDestroySessionCompleted(FName SessionName, bool Successful)
{
	if (const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld()))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}

	OnDestroySessionCompleteEvent.Broadcast(Successful);
}

void USessionSubsystem::FindSessions(int32 MaxSearchResults, bool IsLANQuery)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	FindSessionsCompleteDelegateHandle =
		SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IsLANQuery;

	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

void USessionSubsystem::OnFindSessionsCompleted(bool Successful)
{
	if (const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld()))
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		OnFindSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), Successful);
		return;
	}

	OnFindSessionsCompleteEvent.Broadcast(LastSessionSearch->SearchResults, Successful);
}

void USessionSubsystem::JoinGameSession(const FOnlineSessionSearchResult& SessionResult)
{
	const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		OnJoinGameSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle =
		SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		OnJoinGameSessionCompleteEvent.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void USessionSubsystem::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (const IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld()))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		FString JoinAddress;
		SessionInterface->GetResolvedConnectString(NAME_GameSession, JoinAddress);

		GetWorld()->GetFirstPlayerController()->ClientTravel(JoinAddress, TRAVEL_Absolute);
	}

	OnJoinGameSessionCompleteEvent.Broadcast(Result);
}