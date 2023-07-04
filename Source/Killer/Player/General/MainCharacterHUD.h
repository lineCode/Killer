#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainCharacterHUD.generated.h"

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

public:
	void ShowRestartTextWidget() const;
	void HideRestartTextWidget() const;
	
	void ShowPauseWidget() const;
	void HidePauseWidget() const;
};
