#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ButtonWidget.generated.h"

class UTextWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonClicked, UButtonWidget*, Button);

UCLASS()
class KILLER_API UButtonWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* Button;

	UPROPERTY(Transient, BlueprintReadWrite, meta=(BindWidgetAnim))
	UWidgetAnimation* HoverAnimation;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextWidget* TextWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button", meta=(MultiLine="true", ExposeOnSpawn="true"))
	FText DisplayText;

	/** If we want to associate button with specific widget, then set this variable on spawn. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Button", meta=(ExposeOnSpawn="true"))
	UUserWidget* AssociatedWidget;

	UFUNCTION()
	void BroadcastOnButtonClicked();

	UFUNCTION()
	void OnButtonHovered();

	UFUNCTION()
	void OnButtonUnhovered();

public:
	UPROPERTY(BlueprintAssignable)
	FOnButtonClicked OnButtonClicked;

	void SetAssociatedWidget(UUserWidget* Widget) { AssociatedWidget = Widget; }
	UUserWidget* GetAssociatedWidget() const { return AssociatedWidget; }
};
