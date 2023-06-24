#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FunctionLibrary.generated.h"

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FEffectsInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects|Particles")
	UNiagaraSystem* Particles;

	/**
	 *  If Niagara System has material parameter called "Material" and this variable initialized,
	 *  then particles will have this material.  
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects|Particles")
	UMaterialInterface* ParticlesMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects|Sound")
	USoundWave* Sound;

	/** Pitch modifies in range [0.95f, 1.0f]; */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects|Sound")
	bool bModifyPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects|Camera")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	UPROPERTY(BlueprintReadWrite, Category="Effects|Camera")
	APlayerCameraManager* PlayerCameraManager;

	UPROPERTY(BlueprintReadWrite, Category="Effects|General")
	FVector Location;

	UPROPERTY(BlueprintReadWrite, Category="Effects|General")
	FRotator Rotation;
};

UCLASS()
class KILLER_API UFunctionLibrary : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static void ActivateEffects(const UObject* WorldContextObject, const FEffectsInfo& EffectsInfo);
};
