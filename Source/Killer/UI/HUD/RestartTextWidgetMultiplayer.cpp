#include "RestartTextWidgetMultiplayer.h"

void URestartTextWidgetMultiplayer::ShowRestartText()
{
	PlayAnimation(RestartTextBlockAnimation, 0.0f, 0, EUMGSequencePlayMode::PingPong);

	RestartTextBlock->SetVisibility(ESlateVisibility::Visible);
}
