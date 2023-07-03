#include "GameModeBaseMultiplayerLobby.h"

#include "Killer/Player/Multiplayer/MainCharacterControllerMultiplayer.h"
#include "Killer/UI/Elements/TimerWidget.h"

AGameModeBaseMultiplayerLobby::AGameModeBaseMultiplayerLobby()
{
	PrimaryActorTick.bCanEverTick = false;

	MinNumPlayers = 2;
	TimeToStartMatch = 30.0f;
}

void AGameModeBaseMultiplayerLobby::StartGame() const
{
	GetWorld()->ServerTravel(TravelMapURL, bAbsolute, bShouldSkipGameNotify);
}

void AGameModeBaseMultiplayerLobby::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GetNumPlayers() >= MinNumPlayers && !GetWorldTimerManager().IsTimerActive(StartMatchTimerHandle))
	{
		GetWorldTimerManager().SetTimer(StartMatchTimerHandle, this, &AGameModeBaseMultiplayerLobby::StartGame,
			TimeToStartMatch, false, TimeToStartMatch);

		for (const auto& Controller : PlayerControllers)
		{
			Controller->Client_ShowTimer(TimeToStartMatch);
		}
	}
}
