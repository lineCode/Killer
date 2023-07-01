#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "PlayersTableWidget.generated.h"

UCLASS()
class KILLER_API UPlayersTableWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UUniformGridPanel* PlayersGrid;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Players Table")
	TSubclassOf<UUserWidget> TextWidgetClass;

	void ClearPlayersTable() const;

	void SetPlayersTableColumns(const FText& Name, const FText& Kills, const FText& Deaths, int32 Row) const;

public:
	void RefreshPlayersTable() const;
};
