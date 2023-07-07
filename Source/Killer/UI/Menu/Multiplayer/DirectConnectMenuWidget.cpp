#include "DirectConnectMenuWidget.h"
#include "Killer/UI/Elements/ButtonWidget.h"
#include "Killer/UI/Elements/TextBoxWidget.h"

void UDirectConnectMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ConnectButton->OnButtonClicked.AddDynamic(this, &UDirectConnectMenuWidget::OnConnectButtonCLicked);
}

void UDirectConnectMenuWidget::OnConnectButtonCLicked(UButtonWidget* Button)
{
	GetWorld()->GetFirstPlayerController()->ClientTravel(AddressTextBox->GetText().ToString(), TRAVEL_Absolute);
}
