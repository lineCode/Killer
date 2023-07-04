#include "MainCharacterControllerMultiplayer.h"
#include "EnhancedInputComponent.h"
#include "MainCharacterHUDMultiplayer.h"
#include "GameFramework/PlayerStart.h"
#include "Killer/Combat/Health/HealthComponent.h"
#include "Killer/Input/InputActionsData.h"
#include "Killer/Player/General/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

void AMainCharacterControllerMultiplayer::Restart(const FInputActionValue& Value)
{
	Server_Restart();
}

void AMainCharacterControllerMultiplayer::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (InputActionsData)
	{
		EnhancedInputComponent->BindAction(InputActionsData->ShowPlayersTable, ETriggerEvent::Started, this,
		                                   &AMainCharacterControllerMultiplayer::ShowScoreboard);
		EnhancedInputComponent->BindAction(InputActionsData->ShowPlayersTable, ETriggerEvent::Completed, this,
		                                   &AMainCharacterControllerMultiplayer::HideScoreboard);
		EnhancedInputComponent->BindAction(InputActionsData->ShowPlayersTable, ETriggerEvent::Canceled, this,
		                                   &AMainCharacterControllerMultiplayer::HideScoreboard);
	}
}

void AMainCharacterControllerMultiplayer::Client_ShowTimer_Implementation(const float Seconds)
{
	if (auto* MultiplayerHUD = GetMultiplayerHUD())
	{
		MultiplayerHUD->ShowTimer(Seconds);
	}
}

void AMainCharacterControllerMultiplayer::Client_HideTimer_Implementation()
{
	if (auto* MultiplayerHUD = GetMultiplayerHUD())
	{
		MultiplayerHUD->HideTimer();
	}
}

void AMainCharacterControllerMultiplayer::Client_ShowTextMessage_Implementation(const FText& Message)
{
	if (auto* MultiplayerHUD = GetMultiplayerHUD())
	{
		MultiplayerHUD->ShowTextMessage(Message);
	}
}

void AMainCharacterControllerMultiplayer::Client_HideTextMessage_Implementation()
{
	if (auto* MultiplayerHUD = GetMultiplayerHUD())
	{
		MultiplayerHUD->HideTextMessage();
	}
}

void AMainCharacterControllerMultiplayer::PauseGame(const FInputActionValue& Value)
{
	if (bIsPaused)
	{
		UnPause();
	}
	else
	{
		Pause();
	}
}

void AMainCharacterControllerMultiplayer::Pause()
{
	if (bIsPaused)
	{
		return;
	}

	bIsPaused = true;

	const FInputModeGameAndUI InputModeGameAndUI;
	SetInputMode(InputModeGameAndUI);

	SetShowMouseCursor(true);

	if (MainCharacter)
	{
		MainCharacter->GetMainCharacterHUD()->ShowPauseWidget();
	}
}

void AMainCharacterControllerMultiplayer::UnPause()
{
	if (!bIsPaused)
	{
		return;
	}

	bIsPaused = false;

	const FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);

	SetShowMouseCursor(false);

	if (MainCharacter)
	{
		MainCharacter->GetMainCharacterHUD()->HidePauseWidget();
	}
}

void AMainCharacterControllerMultiplayer::Server_Restart_Implementation()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (!MainCharacter || (MainCharacter && !MainCharacter->GetHealthComponent()->IsDead()))
	{
		return;
	}

	TArray<AActor*> AllPlayerSpawns;
	UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), AllPlayerSpawns);

	if (AllPlayerSpawns.Num() > 0)
	{
		MainCharacter->SetActorLocation(
			AllPlayerSpawns[FMath::RandRange(0, AllPlayerSpawns.Num() - 1)]->GetActorLocation());
	}

	MainCharacter->GetHealthComponent()->Server_ReviveOwner();
}

void AMainCharacterControllerMultiplayer::ShowScoreboard(const FInputActionValue& Value)
{
	if (MainCharacter)
	{
		if (auto* MultiplayerHUD = GetMultiplayerHUD())
		{
			MultiplayerHUD->ShowScoreboardWidget();
		}
	}
}

void AMainCharacterControllerMultiplayer::HideScoreboard(const FInputActionValue& Value)
{
	if (auto* MultiplayerHUD = GetMultiplayerHUD())
	{
		MultiplayerHUD->HideScoreboardWidget();
	}
}

AMainCharacterHUDMultiplayer* AMainCharacterControllerMultiplayer::GetMultiplayerHUD() const
{
	return Cast<AMainCharacterHUDMultiplayer>(GetHUD());
}
