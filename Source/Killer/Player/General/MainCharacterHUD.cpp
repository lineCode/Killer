﻿#include "MainCharacterHUD.h"
#include "MainCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Killer/UI/HUD/RestartTextWidget.h"
#include "Killer/UI/Menu/Pause/PauseMenuWidget.h"

AMainCharacterHUD::AMainCharacterHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMainCharacterHUD::BeginPlay()
{
	Super::BeginPlay();

	if (auto* MainCharacter = Cast<AMainCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		MainCharacter->SetMainCharacterHUD(this);
	}

	RestartTextWidget = CreateWidget<URestartTextWidget>(GetWorld(), RestartTextWidgetClass);
	RestartTextWidget->SetOwningPlayer(GetOwningPlayerController());

	PauseMenuWidget = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseMenuWidgetClass);
	PauseMenuWidget->SetOwningPlayer(GetOwningPlayerController());
}

void AMainCharacterHUD::Destroyed()
{
	Super::Destroyed();

	HideRestartTextWidget(); // Without this, restart text widget pops up when player exits to the main menu. I don't know why.
}

void AMainCharacterHUD::ShowRestartTextWidget() const
{
	RestartTextWidget->ShowRestartText();
	
	RestartTextWidget->AddToViewport();
}

void AMainCharacterHUD::HideRestartTextWidget() const
{
	RestartTextWidget->RemoveFromParent();
}

void AMainCharacterHUD::ShowPauseWidget() const
{
	PauseMenuWidget->AddToViewport();
}

void AMainCharacterHUD::HidePauseWidget() const
{
	PauseMenuWidget->RemoveFromParent();
}
