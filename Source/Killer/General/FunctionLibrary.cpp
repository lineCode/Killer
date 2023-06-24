#include "FunctionLibrary.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

void UFunctionLibrary::ActivateEffects(const UObject* WorldContextObject, const FEffectsInfo& EffectsInfo)
{
	if (EffectsInfo.Particles)
	{
		if (UNiagaraComponent* Particles = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			WorldContextObject, EffectsInfo.Particles, EffectsInfo.Location, EffectsInfo.Rotation); EffectsInfo.ParticlesMaterial)
		{
			Particles->SetVariableMaterial("Material", EffectsInfo.ParticlesMaterial);
		}
	}

	if (EffectsInfo.Sound)
	{
		float PitchMultiplier = 1.0f;

		if (EffectsInfo.bModifyPitch)
		{
			PitchMultiplier = FMath::RandRange(0.95f, 1.05f);
		}

		UGameplayStatics::PlaySoundAtLocation(WorldContextObject, EffectsInfo.Sound, EffectsInfo.Location, 1.0f, PitchMultiplier);
	}

	if (EffectsInfo.CameraShakeClass && EffectsInfo.PlayerCameraManager)
	{
		EffectsInfo.PlayerCameraManager->StartCameraShake(EffectsInfo.CameraShakeClass);
	}
}
