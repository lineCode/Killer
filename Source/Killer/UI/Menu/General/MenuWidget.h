#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

UCLASS()
class KILLER_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	UMenuWidget* OwningWidget;

public:
	UMenuWidget* GetOwningWidget() const { return OwningWidget; }
	void SetOwningWidget(UMenuWidget* Widget) { OwningWidget = Widget; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Return();

	virtual void Return_Implementation();
};
