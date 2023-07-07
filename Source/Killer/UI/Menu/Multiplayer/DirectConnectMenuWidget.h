#pragma once

#include "CoreMinimal.h"
#include "Killer/UI/Menu/General/MenuWidget.h"
#include "DirectConnectMenuWidget.generated.h"

class UButtonWidget;
class UTextBoxWidget;

UCLASS()
class KILLER_API UDirectConnectMenuWidget : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBoxWidget* AddressTextBox;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButtonWidget* ConnectButton;

	UFUNCTION()
	void OnConnectButtonCLicked(UButtonWidget* Button);
};
