#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class AGun;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KILLER_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Component")
	bool bLoadFromSave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Component", meta=(EditCondition="!bLoadFromSave"))
	TSubclassOf<AGun> WeaponClass;

	UPROPERTY()
	AActor* Owner;

	UPROPERTY()
	APlayerController* OwnerController;

	UPROPERTY()
	AGun* Gun;

	void MoveWeapon(); 
	void RotateWeapon();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	AGun* GetGun() const { return Gun; }

	UFUNCTION(NetMulticast, Reliable)
	void SpawnWeaponMulticast();

	UFUNCTION(Server, Unreliable)
	void MoveWeaponMulticast(const FVector& Location);

	UFUNCTION(Server, Unreliable)
	void RotateWeaponMulticast(const FQuat& Rotation);
};
