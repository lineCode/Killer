#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainCharacterHUD.generated.h"

class UPauseMenuWidget;
class UHUDWidget;

UCLASS()
class KILLER_API AMainCharacterHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMainCharacterHUD();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Main Character HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UHUDWidget* HUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Main Character HUD")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UPauseMenuWidget* PauseMenuWidget;

public:
	UHUDWidget* GetHUDWidget() const { return HUDWidget; }

	void ShowPauseWidget() const;
	void HidePauseWidget() const;
};
