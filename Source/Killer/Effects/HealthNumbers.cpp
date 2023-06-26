#include "HealthNumbers.h"
#include "Killer/UI/HealthNumbersWidget.h"

AHealthNumbers::AHealthNumbers()
{
    PrimaryActorTick.bCanEverTick = false;

    NumbersWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Numbers Widget Component"));
    NumbersWidgetComponent->SetupAttachment(RootComponent);
}

void AHealthNumbers::ShowHealthNumbers_Implementation(const float Value) const
{
    if (UUserWidget* UserWidget = NumbersWidgetComponent->GetWidget())
    {
        if (UHealthNumbersWidget* HealthNumbersWidget = Cast<UHealthNumbersWidget>(UserWidget))
        {
            HealthNumbersWidget->ShowHealthNumbers(Value, Color);
        }
    }
}
