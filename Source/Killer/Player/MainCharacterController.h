#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Killer/General/FunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "MainCharacterController.generated.h"

UCLASS()
class KILLER_API AMainCharacterController : public APlayerController
{
	GENERATED_BODY()

private:
	void MoveRight(float Value);

	void Jump();
	void StopJumping();

	void Shoot();

	void Restart();

	bool CheckIfDead();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		UNiagaraSystem* JumpParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		USoundWave* JumpSound;
	
public:
	virtual void SetupInputComponent() override;
};
