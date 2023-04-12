#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ParticlesAndSound.generated.h"

UCLASS()
class KILLER_API AParticlesAndSound : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		UNiagaraSystem* Particles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		USoundWave* Sound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		bool RandomizePitch;
	
public:	
	AParticlesAndSound();

	UFUNCTION(Server, UnReliable)
		void SpawnParticlesAndSoundServer();

	UFUNCTION(NetMulticast, UnReliable)
		void SpawnParticlesAndSoundMulticast();
};
