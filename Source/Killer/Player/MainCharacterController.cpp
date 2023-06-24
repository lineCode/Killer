#include "MainCharacterController.h"
#include "GameFramework/Character.h"
#include "MainCharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Killer/Weapons/Gun.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Killer/Weapons/WeaponComponent.h"

AMainCharacterController::AMainCharacterController()
{
    CanShoot = true;
}

void AMainCharacterController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    MainCharacter = Cast<AMainCharacter>(InPawn);

    SetShowMouseCursor(false);
    UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
}

void AMainCharacterController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAxis("MoveRight", this, &AMainCharacterController::MoveRight);

    InputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacterController::Jump);
    InputComponent->BindAction("Jump", IE_Released, this, &AMainCharacterController::StopJumping);

    InputComponent->BindAxis("Shoot", this, &AMainCharacterController::Shoot);

    InputComponent->BindAction("Restart", IE_Pressed, this, &AMainCharacterController::Restart);
}

void AMainCharacterController::MoveRight(const float Value)
{
    if (!MainCharacter || (MainCharacter && MainCharacter->GetHealthComponent()->IsDead()))
    {
        return;
    }
    
    if (!IsInputEnabled)
    {
        return;
    }

    MainCharacter->AddMovementInput(FVector(1, 0, 0), Value * Speed);
}

void AMainCharacterController::Jump()
{
    if (!MainCharacter || (MainCharacter && MainCharacter->GetHealthComponent()->IsDead()))
    {
        return;
    }
    
    if (!IsInputEnabled)
    {
        return;
    }

    ACharacter* OwnerCharacter = GetCharacter();
    if (!OwnerCharacter)
    {
        return;
    }

    if (OwnerCharacter->CanJump())
    {
        FVector EffectsLocation = OwnerCharacter->GetActorLocation();
        EffectsLocation.Z -= OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

        JumpEffectsInfo.Location = EffectsLocation;
        JumpEffectsInfo.Rotation = FRotator(0.0f, -1.0f, 0.0f);
    
        UFunctionLibrary::ActivateEffects(this, JumpEffectsInfo);
    }

    OwnerCharacter->Jump();
}

void AMainCharacterController::StopJumping()
{
    if (!MainCharacter || (MainCharacter && MainCharacter->GetHealthComponent()->IsDead()))
    {
        return;
    }
    
    if (!IsInputEnabled)
    {
        return;
    }

    GetCharacter()->StopJumping();
}

void AMainCharacterController::Shoot(const float Value)
{
    FireGunServer(Value);
}

void AMainCharacterController::Restart()
{
    const UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    if (!MainCharacter || (MainCharacter && !MainCharacter->GetHealthComponent()->IsDead()))
    {
        return;
    }

    UGameplayStatics::OpenLevel(World, FName(UGameplayStatics::GetCurrentLevelName(World)));
}

void AMainCharacterController::FireGunServer_Implementation(const float Value)
{
    if (!MainCharacter || (MainCharacter && MainCharacter->GetHealthComponent()->IsDead()))
    {
        return;
    }
    
    if (!CanShoot && Value <= 0.0f)
    {
        CanShoot = true;
    }

    if (Value <= 0.0f || !IsInputEnabled || !CanShoot)
    {
        return;
    }

    AGun* Gun = MainCharacter->GetWeaponComponent()->GetGun();
    if (!Gun)
    {
        return;
    }

    Gun->FireFromMuzzle(MainCharacter->BulletModifiers);

    if (!Gun->IsAutomatic())
    {
        CanShoot = false;
    }
}