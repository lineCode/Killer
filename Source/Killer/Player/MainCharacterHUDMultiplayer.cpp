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

	if (AGameStateBase* MultiplayerGameState = UGameplayStatics::GetGameState(this))
	{
		for (const auto Player : MultiplayerGameState->PlayerArray)
		{
			if (const auto* MultiplayerController = Cast<AMainCharacterControllerMultiplayer>(Player->GetPlayerController()))
			{
				if (const auto* MultiplayerHUD = Cast<AMainCharacterHUDMultiplayer>(MultiplayerController->GetHUD()))
				{
					MultiplayerHUD->GetPlayersTableWidget()->RefreshPlayersTable();
				}
			}
		}
	}
}

void AMainCharacterHUDMultiplayer::ShowPlayersTableWidget() const
{
	PlayersTableWidget->AddToViewport();
}

void AMainCharacterHUDMultiplayer::HidePlayersTableWidget() const
{
	PlayersTableWidget->RemoveFromParent();
}