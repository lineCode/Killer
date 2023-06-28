#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateMultiplayer.generated.h"

UCLASS()
class KILLER_API APlayerStateMultiplayer : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Player Info")
	FText PlayerDisplayName;

	UPROPERTY(ReplicatedUsing="OnRep_KillsCount", EditDefaultsOnly, BlueprintReadWrite, Category="Player Info")
	int32 KillsCount;

	UFUNCTION()
	void OnRep_KillsCount();

public:
	APlayerStateMultiplayer();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	int32 GetKillsCount() const { return KillsCount; }
	
	UFUNCTION(Server, Reliable)
	void Server_IncrementKillsCount();
};
