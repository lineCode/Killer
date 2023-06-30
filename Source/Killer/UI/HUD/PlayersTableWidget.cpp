#include "PlayersTableWidget.h"
#include "GameFramework/GameStateBase.h"
#include "Killer/Player/PlayerStateMultiplayer.h"
#include "Killer/UI/Elements/TextWidget.h"
#include "Kismet/GameplayStatics.h"

void UPlayersTableWidget::RefreshPlayersTable() const
{
	AGameStateBase* GameStateBase = UGameplayStatics::GetGameState(GetOwningPlayer());
	if (!GameStateBase)
	{
		return;
	}

	ClearPlayersTable();

	int32 Rows = 1;
	for (auto& Player : GameStateBase->PlayerArray)
	{
		if (const auto* MultiplayerPlayerState = Cast<APlayerStateMultiplayer>(Player))
		{
			Rows++;
			int32 Columns = 0;
			if (UTextWidget* TextWidget = CreateWidget<UTextWidget>(GetOwningPlayer(), TextWidgetClass))
			{
				PlayersGrid->AddChildToUniformGrid(TextWidget, Rows, Columns++);

				TextWidget->SetDisplayText(FText::FromString(MultiplayerPlayerState->GetPlayerName()));
			}
			if (UTextWidget* TextWidget = CreateWidget<UTextWidget>(GetOwningPlayer(), TextWidgetClass))
			{
				PlayersGrid->AddChildToUniformGrid(TextWidget, Rows, Columns++);

				TextWidget->SetDisplayText(FText::FromString(FString::FromInt(MultiplayerPlayerState->GetKillsCount())));
			}
			if (UTextWidget* TextWidget = CreateWidget<UTextWidget>(GetOwningPlayer(), TextWidgetClass))
			{
				PlayersGrid->AddChildToUniformGrid(TextWidget, Rows, Columns++);

				TextWidget->SetDisplayText(FText::FromString(FString::FromInt(MultiplayerPlayerState->GetDeathsCount())));
			}
		}
	}
}

void UPlayersTableWidget::ClearPlayersTable() const
{
	PlayersGrid->ClearChildren();

	int32 Columns = 0;
	if (UTextWidget* TextWidget = CreateWidget<UTextWidget>(GetOwningPlayer(), TextWidgetClass))
	{
		PlayersGrid->AddChildToUniformGrid(TextWidget, 0, Columns++);

		TextWidget->SetDisplayText(FText::FromString("Nickname"));
	}
	if (UTextWidget* TextWidget = CreateWidget<UTextWidget>(GetOwningPlayer(), TextWidgetClass))
	{
		PlayersGrid->AddChildToUniformGrid(TextWidget, 0, Columns++);

		TextWidget->SetDisplayText(FText::FromString("Kills"));
	}
	if (UTextWidget* TextWidget = CreateWidget<UTextWidget>(GetOwningPlayer(), TextWidgetClass))
	{
		PlayersGrid->AddChildToUniformGrid(TextWidget, 0, Columns++);

		TextWidget->SetDisplayText(FText::FromString("Deaths"));
	}
}
