#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "RestartTextWidget.generated.h"

class UScoreboardWidget;

UCLASS()
class KILLER_API URestartTextWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* RestartTextBlock;

	UPROPERTY(Transient, BlueprintReadWrite, meta=(BindWidgetAnim))
	UWidgetAnimation* RestartTextBlockAnimation;

public:
	virtual void ShowDeathText();
	virtual void HideDeathText();
};
