#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "EffectsActor.generated.h"

UCLASS()
class KILLER_API AEffectsActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UAudioComponent* AudioComponent;

	UPROPERTY(ReplicatedUsing="OnRep_ParticlesMaterial")
	UMaterialInterface* ParticlesMaterial;

	UFUNCTION()
	void OnRep_ParticlesMaterial() const;

public:
	AEffectsActor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Server, Reliable)
	void Server_SetParticlesMaterial(UMaterialInterface* Material);
};
