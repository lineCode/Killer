#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class AMainCharacterController;
class AMainCharacter;
class AGun;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KILLER_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Component")
	bool bLoadFromSave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Component", meta=(EditCondition="!bLoadFromSave"))
	TSubclassOf<AGun> WeaponClass;

	UPROPERTY(Replicated)
	AMainCharacter* MainCharacterOwner;

	UPROPERTY(Replicated)
	AMainCharacterController* MainCharacterController;

	UPROPERTY(Replicated)
	AGun* Gun;

	void SpawnWeapon();

	UFUNCTION(Server, Unreliable)
	void Server_MoveWeapon(const FVector& Location) const;

	UFUNCTION(Server, Unreliable)
	void Server_RotateWeapon() const;

	void MoveWeapon() const;

public:
	UWeaponComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintPure, Category="Weapon")
	FORCEINLINE AGun* GetGun() const { return Gun; }

	void SetMainCharacterController(AMainCharacterController* Controller) { MainCharacterController = Controller; }
};
