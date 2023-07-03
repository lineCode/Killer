#pragma once

#include "CoreMinimal.h"
#include "GameModeBaseMultiplayer.h"
#include "GameModeBaseMultiplayerLobby.generated.h"

UCLASS()
class KILLER_API AGameModeBaseMultiplayerLobby : public AGameModeBaseMultiplayer
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game Mode|Players")
	int32 MinNumPlayers;

	/** Time to start match in seconds. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game Mode|Travel")
	float TimeToStartMatch;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game Mode|Travel")
	FString TravelMapURL;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game Mode|Travel")
	bool bAbsolute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game Mode|Travel")
	bool bShouldSkipGameNotify;

	FTimerHandle StartMatchTimerHandle;
	void StartGame() const;

public:
	AGameModeBaseMultiplayerLobby();
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
