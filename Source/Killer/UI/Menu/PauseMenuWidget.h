#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "PauseMenuWidget.generated.h"

class UButtonWidget;

UCLASS()
class KILLER_API UPauseMenuWidget : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UVerticalBox* SettingsMenuBox;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButtonWidget* ContinueButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButtonWidget* SettingsButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButtonWidget* MainMenuButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButtonWidget* QuitButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> MainMenuLevel;

	UFUNCTION()
	void OnContinueButtonClicked(UButtonWidget* Button);

	UFUNCTION()
	void OnSettingsButtonClicked(UButtonWidget* Button);

	UFUNCTION()
	void OnMainMenuButtonClicked(UButtonWidget* Button);

	UFUNCTION()
	void OnQuitButtonClicked(UButtonWidget* Button);
};
