#pragma once

#include "CoreMinimal.h"
#include "Killer/Player/General/MainCharacterController.h"
#include "MainCharacterControllerMultiplayer.generated.h"

class AMainCharacterHUDMultiplayer;

UCLASS()
class KILLER_API AMainCharacterControllerMultiplayer : public AMainCharacterController
{
	GENERATED_BODY()

protected:
	virtual void Restart(const FInputActionValue& Value) override;

	UFUNCTION(Server, Reliable)
	void Server_Restart();

	bool bIsPaused;

	void ShowScoreboard(const FInputActionValue& Value);
	void HideScoreboard(const FInputActionValue& Value);

	AMainCharacterHUDMultiplayer* GetMultiplayerHUD() const;

public:
	virtual void SetupInputComponent() override;

	UFUNCTION(Client, Reliable)
	void Client_ShowTimer(float Seconds);

	UFUNCTION(Client, Reliable)
	void Client_HideTimer();

	UFUNCTION(Client, Reliable)
	void Client_ShowTextMessage(const FText& Message);

	UFUNCTION(Client, Reliable)
	void Client_HideTextMessage();

	void PauseGame(const FInputActionValue& Value) override;
	virtual void Pause() override;
	virtual void UnPause() override;
};
