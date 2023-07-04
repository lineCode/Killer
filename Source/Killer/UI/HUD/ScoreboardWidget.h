#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "ScoreboardWidget.generated.h"

UCLASS()
class KILLER_API UScoreboardWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UUniformGridPanel* PlayersGrid;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Players Table")
	TSubclassOf<UUserWidget> TextWidgetClass;

	void ClearScoreboard() const;

	void SetScoreboardColumns(const FText& Name, const FText& Kills, const FText& Deaths, int32 Row) const;

public:
	void RefreshScoreboard() const;
};
