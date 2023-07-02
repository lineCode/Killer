#pragma once

#include "CoreMinimal.h"
#include "BulletInfo.generated.h"

USTRUCT(BlueprintType)
struct FBulletInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float MinDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float MaxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageMultiplier;

	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float ImpulseForce;
};
