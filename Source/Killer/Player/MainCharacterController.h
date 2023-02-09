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
	bool CanShoot;

	void MoveRight(float Value);

	void Jump();
	void StopJumping();

	void Shoot(float Value);

	void Restart();

	bool CheckIfDead();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		UNiagaraSystem* JumpParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		USoundWave* JumpSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
		bool IsInputEnabled;
	
public:
	AMainCharacterController();

	virtual void SetupInputComponent() override;
};
