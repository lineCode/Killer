#include "RestartTextWidgetMultiplayer.h"

void URestartTextWidgetMultiplayer::ShowDeathText()
{
	PlayAnimation(RestartTextBlockAnimation, 0.0f, 0, EUMGSequencePlayMode::PingPong);

	RestartTextBlock->SetVisibility(ESlateVisibility::Visible);
}
