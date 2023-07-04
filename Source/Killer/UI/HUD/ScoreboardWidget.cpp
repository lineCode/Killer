#include "ScoreboardWidget.h"
#include "GameFramework/GameStateBase.h"
#include "Killer/Player/Multiplayer/MainCharacterStateMultiplayer.h"
#include "Killer/UI/Elements/TextWidget.h"
#include "Kismet/GameplayStatics.h"

void UScoreboardWidget::RefreshScoreboard() const
{
	AGameStateBase* GameStateBase = UGameplayStatics::GetGameState(GetOwningPlayer());
	if (!GameStateBase)
	{
		return;
	}

	ClearScoreboard();

	int32 Rows = 1;
	for (auto& Player : GameStateBase->PlayerArray)
	{
		if (const auto* MultiplayerPlayerState = Cast<AMainCharacterStateMultiplayer>(Player))
		{
			Rows++;

			SetScoreboardColumns(FText::FromString(MultiplayerPlayerState->GetPlayerName()),
			                       FText::FromString(FString::FromInt(MultiplayerPlayerState->GetKillsCount())),
			                       FText::FromString(FString::FromInt(MultiplayerPlayerState->GetDeathsCount())),
			                       Rows);
		}
	}
}

void UScoreboardWidget::ClearScoreboard() const
{
	PlayersGrid->ClearChildren();

	SetScoreboardColumns(FText::FromString("Name"), FText::FromString("Kills"), FText::FromString("Deaths"), 0);
}

void UScoreboardWidget::SetScoreboardColumns(const FText& Name, const FText& Kills, const FText& Deaths,
                                                 const int32 Row) const
{
	int32 Columns = 0;

	if (UTextWidget* TextWidget = CreateWidget<UTextWidget>(GetOwningPlayer(), TextWidgetClass))
	{
		PlayersGrid->AddChildToUniformGrid(TextWidget, Row, Columns++);

		TextWidget->SetDisplayText(Name);
	}
	if (UTextWidget* TextWidget = CreateWidget<UTextWidget>(GetOwningPlayer(), TextWidgetClass))
	{
		PlayersGrid->AddChildToUniformGrid(TextWidget, Row, Columns++);

		TextWidget->SetDisplayText(Kills);
	}
	if (UTextWidget* TextWidget = CreateWidget<UTextWidget>(GetOwningPlayer(), TextWidgetClass))
	{
		PlayersGrid->AddChildToUniformGrid(TextWidget, Row, Columns++);

		TextWidget->SetDisplayText(Deaths);
	}
}
