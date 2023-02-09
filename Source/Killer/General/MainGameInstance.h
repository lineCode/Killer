#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

class AGun;

UCLASS()
class KILLER_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AGun> GunClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Levelname;
};
