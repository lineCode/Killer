#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionWidget.generated.h"

class UButtonWidget;
class UTextWidget;

UCLASS()
class KILLER_API UMultiplayerSessionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextWidget* ServerNameText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextWidget* NumPlayersText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextWidget* PingText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButtonWidget* ConnectButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Session Widget")
	int32 MaxServerNameLength;

	UFUNCTION()
	void OnConnectButtonClicked(UButtonWidget* Button);

	FOnlineSessionSearchResult Session;

	void OnJoinGameSessionComplete(EOnJoinSessionCompleteResult::Type Result);

public:
	void DisplaySessionInfo(const FOnlineSessionSearchResult& SessionSearchResult);
};
