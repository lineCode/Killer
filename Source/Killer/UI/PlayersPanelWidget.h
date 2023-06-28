#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PlayersPanelWidget.generated.h"

UCLASS()
class KILLER_API UPlayersPanelWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* KillsCountTextBox;

public:
	void DisplayKillsCount(int32 Value) const;
};
