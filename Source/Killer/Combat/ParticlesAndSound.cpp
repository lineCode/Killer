#include "ParticlesAndSound.h"

void AParticlesAndSound::BeginPlay()
{
	Super::BeginPlay();

	SpawnParticlesAndSoundServer();
}

AParticlesAndSound::AParticlesAndSound()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
}

void AParticlesAndSound::SpawnParticlesAndSoundServer_Implementation()
{
	SpawnParticlesAndSoundMulticast();
}

void AParticlesAndSound::SpawnParticlesAndSoundMulticast_Implementation()
{
	UWorld* World = GetWorld();
	if (!World) return;

	if (Particles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, Particles, GetActorLocation(), GetActorRotation());
	}

	if (Sound)
	{
		float PitchMultiplier = 1.0f;

		if (RandomizePitch)
		{
			PitchMultiplier = FMath::RandRange(0.95f, 1.05f);
		}

		UGameplayStatics::PlaySoundAtLocation(World, Sound, GetActorLocation(), 1.0f, PitchMultiplier);
	}
}

