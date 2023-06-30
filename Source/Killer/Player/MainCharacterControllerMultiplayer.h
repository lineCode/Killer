#pragma once

#include "CoreMinimal.h"
#include "MainCharacterController.h"
#include "MainCharacterControllerMultiplayer.generated.h"

UCLASS()
class KILLER_API AMainCharacterControllerMultiplayer : public AMainCharacterController
{
	GENERATED_BODY()

protected:
	virtual void Restart(const FInputActionValue& Value) override;

	UFUNCTION(Server, Reliable)
	void Server_Restart();

	bool bIsPaused;

	void ShowPlayersTable(const FInputActionValue& Value);
	void HidePlayersTable(const FInputActionValue& Value);

public:
	virtual void SetupInputComponent() override;
	
	virtual void Pause() override;
	virtual void UnPause() override;
};
