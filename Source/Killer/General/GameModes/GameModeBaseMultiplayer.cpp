﻿#include "GameModeBaseMultiplayer.h"
#include "Killer/Player/Multiplayer/MainCharacterControllerMultiplayer.h"

AGameModeBaseMultiplayer::AGameModeBaseMultiplayer()
{
	PrimaryActorTick.bCanEverTick = false;

	MaxNumPlayers = 4;
}

void AGameModeBaseMultiplayer::PreLogin(const FString& Options, const FString& Address,
	const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	if (PlayerControllers.Num() >= MaxNumPlayers)
	{
		ErrorMessage = TEXT("Server full.");
		FGameModeEvents::GameModePreLoginEvent.Broadcast(this, UniqueId, ErrorMessage);
		
		return;
	}
	
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void AGameModeBaseMultiplayer::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	PlayerControllers.Add(Cast<AMainCharacterControllerMultiplayer>(NewPlayer));
}

void AGameModeBaseMultiplayer::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	PlayerControllers.Remove(Cast<AMainCharacterControllerMultiplayer>(Exiting));
}
