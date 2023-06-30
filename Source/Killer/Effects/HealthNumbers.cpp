#include "HealthNumbers.h"
#include "Killer/UI/Game/HealthNumbersWidget.h"
#include "Net/UnrealNetwork.h"

AHealthNumbers::AHealthNumbers()
{
    PrimaryActorTick.bCanEverTick = false;

    NumbersWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Numbers Widget Component"));
    NumbersWidgetComponent->SetupAttachment(RootComponent);
}

void AHealthNumbers::BeginPlay()
{
    Super::BeginPlay();

    if (UUserWidget* UserWidget = NumbersWidgetComponent->GetWidget())
    {
        if (auto* HealthNumbersWidget = Cast<UHealthNumbersWidget>(UserWidget))
        {
            HealthNumbersWidget->ShowHealthNumbers(HealthValue, Color);
        }
    }
}

void AHealthNumbers::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHealthNumbers, HealthValue);
}
