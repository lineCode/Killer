#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "MainCharacterController.generated.h"

class UGameplayAbility;
class UInputActionsData;
class AEffectsActor;
class AMainCharacter;

UCLASS()
class KILLER_API AMainCharacterController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	AMainCharacter* MainCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller|Abilities")
	TSubclassOf<UGameplayAbility> ShootAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller|Input")
	bool bIsInputEnabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Controller|Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Controller|Input")
	UInputActionsData* InputActionsData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller|Move")
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller|Effects")
	TSubclassOf<AEffectsActor> JumpEffectsActorClass;

	void Move(const FInputActionValue& Value);
	void ShootAutomaticGun(const FInputActionValue& Value);
	void ShootNonAutomaticGun(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void StopJumping(const FInputActionValue& Value);
	virtual void PauseGame(const FInputActionValue& Value);
	virtual void Restart(const FInputActionValue& Value);

	UFUNCTION(Server, Unreliable)
	void Server_SpawnJumpEffects();

	bool CanProceedInput() const;

	bool IsGunAutomatic() const;

public:
	AMainCharacterController();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetupInputComponent() override;

	virtual void Pause() override;
	virtual void UnPause();
};
