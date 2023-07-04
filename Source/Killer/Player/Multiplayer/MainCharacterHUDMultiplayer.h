#pragma once

#include "CoreMinimal.h"
#include "Killer/Player/General/MainCharacterHUD.h"
#include "MainCharacterHUDMultiplayer.generated.h"

class UTextWidget;
class UTimerWidget;
class UScoreboardWidget;

UCLASS()
class KILLER_API AMainCharacterHUDMultiplayer : public AMainCharacterHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Main Character HUD|Multiplayer")
	TSubclassOf<UUserWidget> ScoreboardWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UScoreboardWidget* ScoreboardWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Main Character HUD|Multiplayer")
	TSubclassOf<UUserWidget> TimerWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UTimerWidget* TimerWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Main Character HUD|Multiplayer")
	TSubclassOf<UUserWidget> TextMessageWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UTextWidget* TextMessageWidget;

public:
	AMainCharacterHUDMultiplayer();

	void ShowScoreboardWidget();
	void HideScoreboardWidget() const;

	void ShowTimer(float Seconds);
	void HideTimer() const;
	
	void ShowTextMessage(const FText& Message);
	void HideTextMessage() const;
};
