#include "ButtonWidget.h"

#include "TextWidget.h"

void UButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	TextWidget->SetDisplayText(DisplayText);
}

void UButtonWidget::NativeConstruct()
{
	Super::NativePreConstruct();

	Button->OnClicked.AddDynamic(this, &UButtonWidget::BroadcastOnButtonClicked);
	Button->OnHovered.AddDynamic(this, &UButtonWidget::OnButtonHovered);
	Button->OnUnhovered.AddDynamic(this, &UButtonWidget::OnButtonUnhovered);
}

void UButtonWidget::BroadcastOnButtonClicked()
{
	OnButtonClicked.Broadcast(this);
}

void UButtonWidget::OnButtonHovered()
{
	PlayAnimation(HoverAnimation, GetAnimationCurrentTime(HoverAnimation));
}

void UButtonWidget::OnButtonUnhovered()
{
	PlayAnimation(HoverAnimation, GetAnimationCurrentTime(HoverAnimation), 1, EUMGSequencePlayMode::Reverse);
}
