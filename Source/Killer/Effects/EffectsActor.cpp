#include "EffectsActor.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"

AEffectsActor::AEffectsActor()
{
	PrimaryActorTick.bCanEverTick = false;

	InitialLifeSpan = 10.0f;

	bReplicates = true;
	AActor::SetReplicateMovement(true);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particles"));
	NiagaraComponent->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound"));
	AudioComponent->SetupAttachment(RootComponent);
}

void AEffectsActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEffectsActor, ParticlesMaterial);
}

void AEffectsActor::Server_SetParticlesMaterial_Implementation(UMaterialInterface* Material)
{
	ParticlesMaterial = Material;

	OnRep_ParticlesMaterial();
}

void AEffectsActor::OnRep_ParticlesMaterial() const
{
	NiagaraComponent->SetVariableMaterial("Material", ParticlesMaterial);
}
