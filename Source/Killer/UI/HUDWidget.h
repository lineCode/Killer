#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "HUDWidget.generated.h"

UCLASS()
class KILLER_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* RestartTextBlock;

	UPROPERTY(Transient, BlueprintReadWrite, meta=(BindWidgetAnim))
	UWidgetAnimation* RestartTextBlockAnimation;

public:
	void ShowDeathText();
};
