#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KillerGameModeBase.generated.h"

UCLASS()
class KILLER_API AKillerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	int32 EnemiesKilled;

public:
	int32 GetEnemiesKilled() const { return EnemiesKilled; }
	void SetEnemiesKilled(const int32 Value) { if (Value >= 0) EnemiesKilled = Value; }
	
	void IncrementEnemiesKilled() { EnemiesKilled++; }
};
