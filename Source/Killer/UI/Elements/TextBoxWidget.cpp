#include "TextBoxWidget.h"
#include "Components/EditableTextBox.h"

void UTextBoxWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (EditableTextBox)
	{
		EditableTextBox->SetText(Text);
	}
}