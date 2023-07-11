#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeNotifyWidget.generated.h"

class UUpgradeUIData;
class UTextWidget;

UCLASS()
class KILLER_API UUpgradeNotifyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextWidget* UpgradeNameText;

	UPROPERTY(Transient, BlueprintReadWrite, meta=(BindWidgetAnim))
	UWidgetAnimation* UpgradeNameTextAnimation;

public:
	void ShowUpgradeNotification(const UUpgradeUIData* UIData);
};
