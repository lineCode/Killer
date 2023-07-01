#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Save.generated.h"

class AGun;

UCLASS()
class KILLER_API USave : public USaveGame
{
	GENERATED_BODY()

public:
	USave();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Volume")
	float EffectsVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Volume")
	float MusicVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Volume")
	float UIVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Volume")
	float FOVShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	bool TutorialCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TSubclassOf<AGun> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	FName LastChosenLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 MaxKillsPerMinute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 TotalKills;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static USave* GetSave();
};
