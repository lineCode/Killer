#include "MainCharacterControllerMultiplayer.h"

#include "EnhancedInputComponent.h"
#include "MainCharacter.h"
#include "MainCharacterHUD.h"
#include "MainCharacterHUDMultiplayer.h"
#include "GameFramework/PlayerStart.h"
#include "Killer/Input/InputActionsData.h"
#include "Killer/UI/HUD/PlayersTableWidget.h"
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
		EnhancedInputComponent->BindAction(InputActionsData->ShowPlayersTable, ETriggerEvent::Started, this, &AMainCharacterControllerMultiplayer::ShowPlayersTable);
		EnhancedInputComponent->BindAction(InputActionsData->ShowPlayersTable, ETriggerEvent::Completed, this, &AMainCharacterControllerMultiplayer::HidePlayersTable);
		EnhancedInputComponent->BindAction(InputActionsData->ShowPlayersTable, ETriggerEvent::Canceled, this, &AMainCharacterControllerMultiplayer::HidePlayersTable);
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
		MainCharacter->SetActorLocation(AllPlayerSpawns[FMath::RandRange(0, AllPlayerSpawns.Num() - 1)]->GetActorLocation());
	}

	MainCharacter->GetHealthComponent()->Server_ReviveOwner();
}

void AMainCharacterControllerMultiplayer::ShowPlayersTable(const FInputActionValue& Value)
{
	if (MainCharacter)
	{
		if (const auto* MultiplayerHUD = Cast<AMainCharacterHUDMultiplayer>(MainCharacter->GetMainCharacterHUD()))
		{
			MultiplayerHUD->ShowPlayersTableWidget();
		}
	}
}

void AMainCharacterControllerMultiplayer::HidePlayersTable(const FInputActionValue& Value)
{
	if (const auto* MultiplayerHUD = Cast<AMainCharacterHUDMultiplayer>(MainCharacter->GetMainCharacterHUD()))
	{
		MultiplayerHUD->HidePlayersTableWidget();
	}
}