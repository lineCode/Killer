#include "RestartTextWidget.h"

#include "Killer/General/Save/Save.h"
#include "Killer/Player/General/MainCharacter.h"
#include "Killer/Player/General/MainCharacterController.h"
#include "Killer/Player/Multiplayer/MainCharacterStateMultiplayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"

void URestartTextWidget::ShowRestartText()
{
	PlayAnimation(RestartTextBlockAnimation, 0.0f, 0, EUMGSequencePlayMode::PingPong);

	const AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetOwningPlayerPawn());
	if (!MainCharacter)
	{
		return;
	}

	const auto* MainCharacterPlayerState = MainCharacter->GetMainCharacterController()->GetPlayerState<
		AMainCharacterStateMultiplayer>();
	if (!MainCharacterPlayerState)
	{
		return;
	}

	const FTextFormat FormattedText(
		FText::FromString("Time: {Time}\nKills: {Kills}\nKills/Min: {KillsPerMin}\nPRESS R"));
	FFormatNamedArguments NamedArguments;

	const float TimeInSeconds = UGameplayStatics::GetTimeSeconds(MainCharacter);

	NamedArguments.Add(TEXT("Time"),
	                   UKismetTextLibrary::AsTimespan_Timespan(UKismetMathLibrary::FromSeconds(TimeInSeconds)));

	const int32 KillsPerMinute = FMath::Floor(MainCharacterPlayerState->GetKillsCount() * 60.0f / TimeInSeconds);

	NamedArguments.Add(TEXT("Kills"), MainCharacterPlayerState->GetKillsCount());
	NamedArguments.Add(TEXT("KillsPerMin"), KillsPerMinute);

	RestartTextBlock->SetText(FText::Format(FormattedText, NamedArguments));
	RestartTextBlock->SetVisibility(ESlateVisibility::Visible);

	if (USave* Save = USave::GetSave())
	{
		if (KillsPerMinute > Save->MaxKillsPerMinute)
		{
			Save->MaxKillsPerMinute = KillsPerMinute;
			UGameplayStatics::SaveGameToSlot(Save, Save->SlotName, 0);
		}
	}
}

void URestartTextWidget::HideDeathText()
{
	StopAnimation(RestartTextBlockAnimation);

	RestartTextBlock->SetVisibility(ESlateVisibility::Hidden);
}
