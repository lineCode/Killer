#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ComboBoxWidget.generated.h"

class UComboBoxString;

UCLASS()
class KILLER_API UComboBoxWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UComboBoxString* ComboBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combo Box Widget")
	TArray<FString> DefaultOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combo Box Widget")
	FString SelectedOption;
};
