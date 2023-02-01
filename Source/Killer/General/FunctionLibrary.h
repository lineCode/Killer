#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Killer/General/Save.h"
#include "FunctionLibrary.generated.h"

UCLASS()
class KILLER_API UFunctionLibrary : public UObject
{
	GENERATED_BODY()
	
public:
	static void SpawnParticlesAndSound(UWorld* World, UNiagaraSystem* NiagaraSystem, USoundWave* SoundWave,
		const FVector& Location, const FRotator& Rotation = FRotator(), bool RandomizePitch = true);

	UFUNCTION(BlueprintCallable)
		static USave* GetSave();
};
