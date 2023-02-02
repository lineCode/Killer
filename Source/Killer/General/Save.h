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
};
