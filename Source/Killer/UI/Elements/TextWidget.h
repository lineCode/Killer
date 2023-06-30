#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "TextWidget.generated.h"

UCLASS()
class KILLER_API UTextWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* TextBlock;

	UPROPERTY(EditAnywhere, BlueprintSetter="SetDisplayText", Category="Text", meta=(MultiLine="true", ExposeOnSpawn="true"))
	FText DisplayText;
	
public:
	UFUNCTION(BlueprintCallable)
	void SetDisplayText(const FText& TextToDisplay);
};
