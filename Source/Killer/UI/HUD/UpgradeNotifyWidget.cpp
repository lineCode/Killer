#include "UpgradeNotifyWidget.h"
#include "Killer/Combat/AbilitySystem/UIData/UpgradeUIData.h"
#include "Killer/UI/Elements/TextWidget.h"

void UUpgradeNotifyWidget::ShowUpgradeNotification(const UUpgradeUIData* UIData)
{
	if (UIData)
	{
		UpgradeNameText->SetDisplayText(UIData->DisplayName);
		PlayAnimation(UpgradeNameTextAnimation, 0, 1, EUMGSequencePlayMode::Forward, 1, true);
	}
}
