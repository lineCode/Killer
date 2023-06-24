#include "HUDWidget.h"

#include "Killer/General/Save.h"
#include "Killer/Player/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"

void UHUDWidget::ShowDeathText()
{
	PlayAnimation(RestartTextBlockAnimation, 0.0f, 0, EUMGSequencePlayMode::PingPong);

	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(OwningPawn))
		{
			const FTextFormat FormattedText(FText::FromString("Time: {Time}\nKills: {Kills}\nKills/Min: {KillsPerMin}\nPRESS R"));
			
			FFormatNamedArguments NamedArguments;
			
			const float TimeInSeconds = UGameplayStatics::GetTimeSeconds(MainCharacter);
			
			NamedArguments.Add(TEXT("Time"),
				UKismetTextLibrary::AsTimespan_Timespan(UKismetMathLibrary::FromSeconds(TimeInSeconds)));
			
			NamedArguments.Add(TEXT("Kills"), MainCharacter->GetKills());

			const int32 KillsPerMinute = FMath::Floor(MainCharacter->GetKills() * 60.0f / TimeInSeconds);
			
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
	}
}
