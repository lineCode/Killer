#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Killer/General/FunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Killer/Combat/ParticlesAndSound.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walk")
		bool IsInputEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		TSubclassOf<AParticlesAndSound> JumpEffects;
	
public:
	AMainCharacterController();

	virtual void SetupInputComponent() override;

	UFUNCTION(Server, Reliable)
		void FireGunServer(const float Value);
};
