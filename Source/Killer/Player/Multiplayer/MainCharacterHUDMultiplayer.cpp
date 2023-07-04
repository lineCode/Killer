#include "MainCharacterHUDMultiplayer.h"

#include "MainCharacterControllerMultiplayer.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Killer/UI/Elements/TextWidget.h"
#include "Killer/UI/Elements/TimerWidget.h"
#include "Killer/UI/HUD/ScoreboardWidget.h"
#include "Kismet/GameplayStatics.h"

AMainCharacterHUDMultiplayer::AMainCharacterHUDMultiplayer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMainCharacterHUDMultiplayer::ShowScoreboardWidget()
{
	if (!ScoreboardWidget)
	{
		ScoreboardWidget = CreateWidget<UScoreboardWidget>(GetOwningPlayerController(), ScoreboardWidgetClass);
		ScoreboardWidget->SetOwningPlayer(GetOwningPlayerController());
	}
	
	ScoreboardWidget->RefreshScoreboard();

	ScoreboardWidget->AddToViewport();
}

void AMainCharacterHUDMultiplayer::HideScoreboardWidget() const
{
	if (ScoreboardWidget)
	{
		ScoreboardWidget->RemoveFromParent();
	}
}

void AMainCharacterHUDMultiplayer::ShowTimer(const float Seconds)
{
	if (!TimerWidget)
	{
		TimerWidget = CreateWidget<UTimerWidget>(GetOwningPlayerController(), TimerWidgetClass);
		TimerWidget->SetOwningPlayer(GetOwningPlayerController());
	}
	
	TimerWidget->StartTimer(Seconds);
	TimerWidget->AddToViewport();
}

void AMainCharacterHUDMultiplayer::HideTimer() const
{
	if (TimerWidget)
	{
		TimerWidget->RemoveFromParent();
	}
}

void AMainCharacterHUDMultiplayer::ShowTextMessage(const FText& Message)
{
	if (!TextMessageWidget)
	{
		TextMessageWidget = CreateWidget<UTextWidget>(GetOwningPlayerController(), TextMessageWidgetClass);
		TextMessageWidget->SetOwningPlayer(GetOwningPlayerController());
	}
	
	TextMessageWidget->SetDisplayText(Message);
	TextMessageWidget->AddToViewport();
}

void AMainCharacterHUDMultiplayer::HideTextMessage() const
{
	if (TextMessageWidget)
	{
		TextMessageWidget->RemoveFromParent();
	}
}
