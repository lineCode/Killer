#include "EffectsActor.h"
#include "Components/AudioComponent.h"

AEffectsActor::AEffectsActor()
{
	PrimaryActorTick.bCanEverTick = false;

	InitialLifeSpan = 10.0f;
	
	bReplicates = true;
	AActor::SetReplicateMovement(true);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particles"));
	NiagaraComponent->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound"));
	AudioComponent->SetupAttachment(RootComponent);
}

