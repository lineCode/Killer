#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeBaseSingleplayer.generated.h"

UCLASS()
class KILLER_API AGameModeBaseSingleplayer : public AGameModeBase
{
	GENERATED_BODY()

protected:
	int32 EnemiesKilled;

public:
	int32 GetEnemiesKilled() const { return EnemiesKilled; }

	void SetEnemiesKilled(const int32 Value)
	{
		if (Value >= 0)
		{
			EnemiesKilled = Value;
		}
	}

	void IncrementEnemiesKilled() { EnemiesKilled++; }
};
