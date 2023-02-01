#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Save.generated.h"

UCLASS()
class KILLER_API USave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SlotName = "SaveSlot";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EffectsVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MusicVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float UIVolume;
};
