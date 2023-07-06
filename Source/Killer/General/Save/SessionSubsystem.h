#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SessionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateSessionCompleted, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateSessionCompleted, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartSessionCompleted, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndSessionCompleted, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroySessionCompleted, bool, Successful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFindSessionsCompleted, const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnJoinSessionCompleted, EOnJoinSessionCompleteResult::Type Result);

UCLASS()
class KILLER_API USessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	private:
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
	FDelegateHandle UpdateSessionCompleteDelegateHandle;

	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	FOnEndSessionCompleteDelegate EndSessionCompleteDelegate;
	FDelegateHandle EndSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;

protected:
	void OnCreateSessionCompleted(FName SessionName, bool Successful);

	void OnUpdateSessionCompleted(FName SessionName, bool Successful);

	void OnStartSessionCompleted(FName SessionName, bool Successful);

	void OnEndSessionCompleted(FName SessionName, bool Successful);

	void OnDestroySessionCompleted(FName SessionName, bool Successful);

	void OnFindSessionsCompleted(bool Successful);

	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

public:
	USessionSubsystem();

	void CreateSession(int32 NumPublicConnections, bool IsLANMatch);
	FOnCreateSessionCompleted OnCreateSessionCompleteEvent;

	void UpdateSession();
	FOnUpdateSessionCompleted OnUpdateSessionCompleteEvent;

	void StartSession();
	FOnStartSessionCompleted OnStartSessionCompleteEvent;

	void EndSession();
	FOnEndSessionCompleted OnEndSessionCompleteEvent;

	void DestroySession();
	FOnDestroySessionCompleted OnDestroySessionCompleteEvent;

	void FindSessions(int32 MaxSearchResults, bool IsLANQuery);
	FOnFindSessionsCompleted OnFindSessionsCompleteEvent;

	void JoinGameSession(const FOnlineSessionSearchResult& SessionResult);
	FOnJoinSessionCompleted OnJoinGameSessionCompleteEvent;
};
