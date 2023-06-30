#include "HUDWidgetMultiplayer.h"

void UHUDWidgetMultiplayer::ShowDeathText()
{
	PlayAnimation(RestartTextBlockAnimation, 0.0f, 0, EUMGSequencePlayMode::PingPong);

	RestartTextBlock->SetVisibility(ESlateVisibility::Visible);
}
