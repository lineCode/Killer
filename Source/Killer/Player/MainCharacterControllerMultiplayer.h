#pragma once

#include "CoreMinimal.h"
#include "MainCharacterController.h"
#include "MainCharacterControllerMultiplayer.generated.h"

UCLASS()
class KILLER_API AMainCharacterControllerMultiplayer : public AMainCharacterController
{
	GENERATED_BODY()

protected:
	virtual void Restart() override;

	UFUNCTION(Server, Reliable)
	void Server_Restart();
};
