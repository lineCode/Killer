#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateMultiplayer.generated.h"

UCLASS()
class KILLER_API APlayerStateMultiplayer : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(ReplicatedUsing="OnRep_KillsCount", EditDefaultsOnly, BlueprintReadWrite, Category="Player Info")
	int32 KillsCount;

	UPROPERTY(ReplicatedUsing="OnRep_DeathsCount", EditDefaultsOnly, BlueprintReadWrite, Category="Player Info")
	int32 DeathsCount;

	UFUNCTION()
	void OnRep_KillsCount();

	UFUNCTION()
	void OnRep_DeathsCount();

	void RefreshPlayersTable() const;

public:
	APlayerStateMultiplayer();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	int32 GetKillsCount() const { return KillsCount; }
	int32 GetDeathsCount() const { return DeathsCount; }
	
	UFUNCTION(Server, Reliable)
	void Server_IncrementKillsCount();

	UFUNCTION(Server, Reliable)
	void Server_IncrementDeathsCount();
};
