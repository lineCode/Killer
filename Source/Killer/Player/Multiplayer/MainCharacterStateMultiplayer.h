#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MainCharacterStateMultiplayer.generated.h"

UCLASS()
class KILLER_API AMainCharacterStateMultiplayer : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, BlueprintReadWrite, Category="Player Info")
	int32 KillsCount;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Player Info")
	int32 DeathsCount;

public:
	AMainCharacterStateMultiplayer();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	int32 GetKillsCount() const { return KillsCount; }
	int32 GetDeathsCount() const { return DeathsCount; }

	UFUNCTION(Server, Reliable)
	void Server_IncrementKillsCount();

	UFUNCTION(Server, Reliable)
	void Server_IncrementDeathsCount();
};
