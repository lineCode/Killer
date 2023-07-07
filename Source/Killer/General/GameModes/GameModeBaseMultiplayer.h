#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeBaseMultiplayer.generated.h"

class AMainCharacterControllerMultiplayer;

UCLASS()
class KILLER_API AGameModeBaseMultiplayer : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game Mode")
	int32 MaxNumPlayers;

public:
	AGameModeBaseMultiplayer();

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void OnPostLogin(AController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	UPROPERTY()
	TArray<AMainCharacterControllerMultiplayer*> PlayerControllers;
};
