#include "DamageNumbers.h"

ADamageNumbers::ADamageNumbers()
{
	PrimaryActorTick.bCanEverTick = false;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	WidgetComponent->SetupAttachment(RootComponent);
}
