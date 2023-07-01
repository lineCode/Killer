#pragma once

#include "CoreMinimal.h"
#include "KillerGameModeBase.h"
#include "KillerGameModeBaseMultiplayer.generated.h"

class AMainCharacterControllerMultiplayer;

UCLASS()
class KILLER_API AKillerGameModeBaseMultiplayer : public AKillerGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void OnPostLogin(AController* NewPlayer) override;

public:
	AKillerGameModeBaseMultiplayer();

	UPROPERTY()
	TArray<AMainCharacterControllerMultiplayer*> PlayerControllers;
};
