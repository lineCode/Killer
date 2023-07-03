#pragma once

#include "CoreMinimal.h"
#include "GameModeBaseMultiplayer.h"
#include "GameModeBaseDeathmatch.generated.h"

UCLASS()
class KILLER_API AGameModeBaseDeathmatch : public AGameModeBaseMultiplayer
{
	GENERATED_BODY()

public:
	AGameModeBaseDeathmatch();

protected:
	/** Time to end match in seconds. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Deathmatch")
	float TimeToEndMatch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Deathmatch")
	float TimeToRestartMatch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Deathmatch")
	TArray<FString> DeathmatchMapsURLs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Deathmatch")
	FText WinMessage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Deathmatch")
	FText LossMessage;

	FTimerHandle EndMatchTimerHandle;
	void EndMatch();

	FTimerHandle RestartMatchTimerHandle;
	void RestartMatch();

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
