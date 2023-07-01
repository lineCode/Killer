#include "MainCharacterHUDMultiplayer.h"

#include "MainCharacterControllerMultiplayer.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Killer/UI/HUD/PlayersTableWidget.h"
#include "Kismet/GameplayStatics.h"

AMainCharacterHUDMultiplayer::AMainCharacterHUDMultiplayer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMainCharacterHUDMultiplayer::BeginPlay()
{
	Super::BeginPlay();

	PlayersTableWidget = CreateWidget<UPlayersTableWidget>(GetWorld(), PlayersTableWidgetClass);
	PlayersTableWidget->SetOwningPlayer(GetOwningPlayerController());
}

void AMainCharacterHUDMultiplayer::ShowPlayersTableWidget() const
{
	PlayersTableWidget->RefreshPlayersTable();

	PlayersTableWidget->AddToViewport();
}

void AMainCharacterHUDMultiplayer::HidePlayersTableWidget() const
{
	PlayersTableWidget->RemoveFromParent();
}
