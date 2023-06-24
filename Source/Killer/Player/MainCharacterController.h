#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Killer/General/FunctionLibrary.h"
#include "MainCharacterController.generated.h"

class AMainCharacter;

UCLASS()
class KILLER_API AMainCharacterController : public APlayerController
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    AMainCharacter* MainCharacter;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller|Move")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller|Input")
    bool IsInputEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller|Effects")
    FEffectsInfo JumpEffectsInfo;

    bool CanShoot;

    void MoveRight(float Value);

    void Jump();
    void StopJumping();

    void Shoot(float Value);

    void Restart();

public:
    AMainCharacterController();
    
    virtual void OnPossess(APawn* InPawn) override;

    virtual void SetupInputComponent() override;

    UFUNCTION(Server, Reliable)
    void FireGunServer(float Value);
};
