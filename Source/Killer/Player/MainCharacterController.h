#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainCharacterController.generated.h"

class AEffectsActor;
class AMainCharacter;

UCLASS()
class KILLER_API AMainCharacterController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void OnPossess(APawn* InPawn) override;
    
    UPROPERTY(Replicated)
    AMainCharacter* MainCharacter;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller|Move")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller|Input")
    bool IsInputEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller|Effects")
    TSubclassOf<AEffectsActor> JumpEffectsActor;

    bool CanShoot;

    void MoveRight(float Value);

    void Jump();
    void StopJumping();

    void Shoot(float Value);

    virtual void Restart();

    UFUNCTION(Server, Unreliable)
    void Server_SpawnJumpEffects();

public:
    AMainCharacterController();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void SetupInputComponent() override;
};
