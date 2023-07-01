#include "KillerGameModeBaseMultiplayer.h"
#include "Killer/Player/Multiplayer/MainCharacterControllerMultiplayer.h"

AKillerGameModeBaseMultiplayer::AKillerGameModeBaseMultiplayer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AKillerGameModeBaseMultiplayer::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	PlayerControllers.Add(Cast<AMainCharacterControllerMultiplayer>(NewPlayer));

	ChangeName(NewPlayer, "Player" + FString::FromInt(GetNumPlayers()), false);
}
