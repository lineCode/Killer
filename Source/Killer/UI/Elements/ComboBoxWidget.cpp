#include "ComboBoxWidget.h"
#include "Components/ComboBoxString.h"

void UComboBoxWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if (ComboBox)
	{
		for (auto& Option : DefaultOptions)
		{
			ComboBox->AddOption(Option);
		}

		ComboBox->SetSelectedOption(SelectedOption);
	}
}