#include "MainCharacterController.h"
#include "GameFramework/Character.h"
#include "MainCharacter.h"
#include "Killer/Weapons/Gun.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"

AMainCharacterController::AMainCharacterController()
{
	CanShoot = true;
}

void AMainCharacterController::MoveRight(float Value)
{
	if (!IsInputEnabled || CheckIfDead()) return;

	GetCharacter()->AddMovementInput(FVector(1, 0, 0), Value * Speed);
}

void AMainCharacterController::Jump()
{
	if (!IsInputEnabled || CheckIfDead()) return;

	ACharacter* OwnerCharacter = GetCharacter();
	if (!OwnerCharacter) return;

	if (OwnerCharacter->CanJump())
	{
		FVector EffectsLocation = OwnerCharacter->GetActorLocation();
		EffectsLocation.Z -= OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		
		UWorld* World = GetWorld();
		if (World)
		{
			World->SpawnActor<AParticlesAndSound>(JumpEffects, EffectsLocation, OwnerCharacter->GetActorRotation());
		}
	}

	OwnerCharacter->Jump();
}

void AMainCharacterController::StopJumping()
{
	if (!IsInputEnabled || CheckIfDead()) return;

	GetCharacter()->StopJumping();
}

void AMainCharacterController::Shoot(float Value)
{
	FireGunServer(Value);
}

void AMainCharacterController::Restart()
{
	UWorld* World = GetWorld();
	if (!World) return;

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetCharacter());
	if (!MainCharacter || !MainCharacter->GetIsDead()) return;

	UGameplayStatics::OpenLevel(World, FName(UGameplayStatics::GetCurrentLevelName(World)));
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

void AMainCharacterController::FireGunServer_Implementation(const float Value)
{
	if (!CanShoot && Value <= 0.0f)
	{
		CanShoot = true;
	}

	if (Value <= 0.0f || !IsInputEnabled || !CanShoot) return;

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetCharacter());
	if (!MainCharacter || MainCharacter->GetIsDead()) return;

	AGun* Gun = MainCharacter->GetGun();
	if (!Gun) return;

	Gun->FireFromMuzzle(MainCharacter->BulletModifiers);

	if (!Gun->IsAutomatic)
	{
		CanShoot = false;
	}
}

bool AMainCharacterController::CheckIfDead()
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetCharacter());
	if (!MainCharacter) return true;

	return MainCharacter->GetIsDead();
}