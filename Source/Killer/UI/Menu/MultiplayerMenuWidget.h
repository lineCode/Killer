#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MultiplayerMenuWidget.generated.h"

class UComboBoxWidget;
class UTextBoxWidget;
class UMultiplayerSessionsMenuWidget;
class UButtonWidget;
class UWidgetSwitcher;
class USessionSubsystem;

UCLASS()
class KILLER_API UMultiplayerMenuWidget : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	USessionSubsystem* SessionSubsystem;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UWidgetSwitcher* WidgetSwitcher;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButtonWidget* HostButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButtonWidget* FindSessionsButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButtonWidget* DirectConnectButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBoxWidget* PlayerNameTextBox;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UComboBoxWidget* PlayerColorComboBox;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UMultiplayerSessionsMenuWidget* SessionsMenuWidget;

	UFUNCTION()
	void OnHostButtonClicked(UButtonWidget* Button);

	UFUNCTION()
	void OnFindSessionsButtonClicked(UButtonWidget* Button);

	UFUNCTION()
	void OnDirectConnectionButtonClicked(UButtonWidget* Button);

	void OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
};
