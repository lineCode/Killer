#include "MainCharacterHUD.h"

#include "MainCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Killer/UI/HUDWidget.h"

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
}