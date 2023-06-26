#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "EffectsActor.generated.h"

UCLASS()
class KILLER_API AEffectsActor : public AActor
{
	GENERATED_BODY()

public:
	AEffectsActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UAudioComponent* AudioComponent;
};
