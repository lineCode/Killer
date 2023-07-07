#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextBoxWidget.generated.h"

class UEditableTextBox;

UCLASS()
class KILLER_API UTextBoxWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UEditableTextBox* EditableTextBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Text Box Widget")
	FText Text;

public:
	FText GetText() { return Text; }
};
