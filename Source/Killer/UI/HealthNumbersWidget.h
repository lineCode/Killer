#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "HealthNumbersWidget.generated.h"

UCLASS()
class KILLER_API UHealthNumbersWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* HealthNumbersTextBlock;

	UPROPERTY(Transient, BlueprintReadWrite, meta=(BindWidgetAnim))
	UWidgetAnimation* HealthNumbersTextBlockAnimation;

public:
	void ShowHealthNumbers(float Value, const FLinearColor& Color);
};
