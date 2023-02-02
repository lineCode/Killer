#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "BulletInfo.generated.h"

USTRUCT(BlueprintType)
struct FBulletInfo
{
	GENERATED_BODY()

public:
	FVector StartDirection;
	AController* InstigatedBy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DamageMultiplier;
};
