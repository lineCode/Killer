#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainCharacterHUD.generated.h"

class UUpgradeNotifyWidget;
class UUpgradeUIData;
class UPauseMenuWidget;
class URestartTextWidget;

UCLASS()
class KILLER_API AMainCharacterHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMainCharacterHUD();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Main Character HUD")
	TSubclassOf<UUserWidget> RestartTextWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	URestartTextWidget* RestartTextWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Main Character HUD")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UPauseMenuWidget* PauseMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Main Character HUD")
	TSubclassOf<UUserWidget> UpgradeNotifyWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUpgradeNotifyWidget* UpgradeNotifyWidget;

public:
	void Destroyed() override;
	
	void ShowRestartTextWidget() const;
	void HideRestartTextWidget() const;
	
	void ShowPauseWidget() const;
	void HidePauseWidget() const;

	void ShowUpgradeNotification(const UUpgradeUIData* UIData);
};
