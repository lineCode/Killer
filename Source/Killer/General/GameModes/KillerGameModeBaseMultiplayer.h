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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game Mode")
	int32 MaxNumPlayers;

public:
	AKillerGameModeBaseMultiplayer();

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void OnPostLogin(AController* NewPlayer) override;

	UPROPERTY()
	TArray<AMainCharacterControllerMultiplayer*> PlayerControllers;
};
