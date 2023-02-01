#include "FunctionLibrary.h"

void UFunctionLibrary::SpawnParticlesAndSound(UWorld* World, UNiagaraSystem* NiagaraSystem, USoundWave* SoundWave,
	const FVector& Location, const FRotator& Rotation, bool RandomizePitch)
{
	if (!World) return;

	if (NiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, NiagaraSystem, Location, Rotation);
	}

	if (SoundWave)
	{
		float PitchMultiplier = 1.0f;

		if (RandomizePitch)
		{
			PitchMultiplier = FMath::RandRange(0.95f, 1.05f);
		}

		UGameplayStatics::PlaySoundAtLocation(World, SoundWave, Location, 1.0f, PitchMultiplier);
	}
}

USave* UFunctionLibrary::GetSave()
{
	USaveGame* EmptySave = UGameplayStatics::CreateSaveGameObject(USave::StaticClass());
	if (!EmptySave) return nullptr;

	USave* Save = Cast<USave>(EmptySave);
	if (!Save) return nullptr;

	if (UGameplayStatics::DoesSaveGameExist(Save->SlotName, 0))
	{
		EmptySave = UGameplayStatics::LoadGameFromSlot(Save->SlotName, 0);
		if (EmptySave)
		{
			Save = Cast<USave>(EmptySave);
		}
	}

	return Save;
}
