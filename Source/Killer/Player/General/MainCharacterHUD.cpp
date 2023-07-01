#include "MainCharacterHUD.h"
#include "MainCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Killer/UI/HUD/HUDWidget.h"
#include "Killer/UI/Menu/PauseMenuWidget.h"

AMainCharacterHUD::AMainCharacterHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMainCharacterHUD::BeginPlay()
{
	Super::BeginPlay();

	HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), HUDWidgetClass);
	HUDWidget->AddToViewport();
	HUDWidget->SetOwningPlayer(GetOwningPlayerController());

	if (auto* MainCharacter = Cast<AMainCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		MainCharacter->SetMainCharacterHUD(this);
	}

	PauseMenuWidget = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseMenuWidgetClass);
	PauseMenuWidget->SetOwningPlayer(GetOwningPlayerController());
}

void AMainCharacterHUD::ShowPauseWidget() const
{
	PauseMenuWidget->AddToViewport();
}

void AMainCharacterHUD::HidePauseWidget() const
{
	PauseMenuWidget->RemoveFromParent();
}
