#pragma once

#include "CoreMinimal.h"
#include "Killer/UI/Menu/General/MenuWidget.h"
#include "MultiplayerSessionsMenuWidget.generated.h"

class UMultiplayerSessionWidget;
class UUniformGridPanel;

UCLASS()
class KILLER_API UMultiplayerSessionsMenuWidget : public UMenuWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UUniformGridPanel* SessionsGrid;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UUserWidget* LoadingCircleWidget;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Multiplayers Sessions")
	TSubclassOf<UMultiplayerSessionWidget> SessionInfoWidgetClass;

public:
	void DisplaySessions(TArray<FOnlineSessionSearchResult> SessionsSearchResults) const;
};
