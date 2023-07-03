#pragma once

#include "CoreMinimal.h"
#include "Killer/Player/General/MainCharacterController.h"
#include "MainCharacterControllerMultiplayer.generated.h"

UCLASS()
class KILLER_API AMainCharacterControllerMultiplayer : public AMainCharacterController
{
	GENERATED_BODY()

protected:
	virtual void Restart(const FInputActionValue& Value) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controller|UI")
	TSubclassOf<UUserWidget> TimerWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controller|UI")
	TSubclassOf<UUserWidget> TextMessageWidgetClass;

	UFUNCTION(Server, Reliable)
	void Server_Restart();

	bool bIsPaused;

	void ShowPlayersTable(const FInputActionValue& Value);
	void HidePlayersTable(const FInputActionValue& Value);

public:
	virtual void SetupInputComponent() override;

	UFUNCTION(Client, Reliable)
	void Client_ShowTimer(float Seconds);

	UFUNCTION(Client, Reliable)
	void Client_ShowTextMessage(const FText& Message);

	virtual void Pause() override;
	virtual void UnPause() override;
};
