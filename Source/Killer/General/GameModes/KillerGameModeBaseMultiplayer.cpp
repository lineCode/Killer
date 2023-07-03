﻿#include "KillerGameModeBaseMultiplayer.h"
#include "Killer/Player/Multiplayer/MainCharacterControllerMultiplayer.h"

AKillerGameModeBaseMultiplayer::AKillerGameModeBaseMultiplayer()
{
	PrimaryActorTick.bCanEverTick = false;

	MaxNumPlayers = 4;
}

void AKillerGameModeBaseMultiplayer::PreLogin(const FString& Options, const FString& Address,
	const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	if (GetNumPlayers() >= MaxNumPlayers)
	{
		ErrorMessage = TEXT("Server full.");
		FGameModeEvents::GameModePreLoginEvent.Broadcast(this, UniqueId, ErrorMessage);
		
		return;
	}
	
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void AKillerGameModeBaseMultiplayer::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	PlayerControllers.Add(Cast<AMainCharacterControllerMultiplayer>(NewPlayer));
}
