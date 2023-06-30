#include "KillerGameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "Player/MainCharacterControllerMultiplayer.h"

void AKillerGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	ChangeName(NewPlayer, "Player", false);
}
