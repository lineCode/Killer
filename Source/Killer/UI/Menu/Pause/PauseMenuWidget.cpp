#include "PauseMenuWidget.h"
#include "Killer/Player/General/MainCharacterController.h"
#include "Killer/UI/Elements/ButtonWidget.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ContinueButton->OnButtonClicked.AddDynamic(this, &UPauseMenuWidget::OnContinueButtonClicked);
	SettingsButton->OnButtonClicked.AddDynamic(this, &UPauseMenuWidget::OnSettingsButtonClicked);
	MainMenuButton->OnButtonClicked.AddDynamic(this, &UPauseMenuWidget::OnMainMenuButtonClicked);
	QuitButton->OnButtonClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitButtonClicked);
}

void UPauseMenuWidget::OnContinueButtonClicked(UButtonWidget* Button)
{
	if (auto* MainCharacterController = Cast<AMainCharacterController>(GetOwningPlayer()))
	{
		MainCharacterController->UnPause();
	}
}

void UPauseMenuWidget::OnSettingsButtonClicked(UButtonWidget* Button)
{
	WidgetSwitcher->SetActiveWidget(SettingsMenuBox);
}

void UPauseMenuWidget::OnMainMenuButtonClicked(UButtonWidget* Button)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetOwningPlayer(), MainMenuLevel);
}

void UPauseMenuWidget::OnQuitButtonClicked(UButtonWidget* Button)
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
