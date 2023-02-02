#include "MainCharacterController.h"
#include "GameFramework/Character.h"
#include "MainCharacter.h"
#include "Killer/Weapons/Gun.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"

void AMainCharacterController::MoveRight(float Value)
{
	if (!CanWalk || CheckIfDead()) return;

	GetCharacter()->AddMovementInput(FVector(1, 0, 0), Value * Speed);
}

void AMainCharacterController::Jump()
{
	if (!CanWalk || CheckIfDead()) return;

	ACharacter* OwnerCharacter = GetCharacter();
	if (!OwnerCharacter) return;

	if (OwnerCharacter->CanJump())
	{
		FVector ParticlesLocation = OwnerCharacter->GetActorLocation();
		ParticlesLocation.Z -= OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		UFunctionLibrary::SpawnParticlesAndSound(GetWorld(), JumpParticles, JumpSound, ParticlesLocation, (-OwnerCharacter->GetActorUpVector()).Rotation());
	}

	OwnerCharacter->Jump();
}

void AMainCharacterController::StopJumping()
{
	if (!CanWalk || CheckIfDead()) return;

	GetCharacter()->StopJumping();
}

void AMainCharacterController::Shoot()
{
	if (!CanShoot) return;

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetCharacter());
	if (!MainCharacter || MainCharacter->GetIsDead()) return;

	AGun* Gun = MainCharacter->GetGun();
	if (!Gun) return;

	Gun->FireFromMuzzle(MainCharacter->BulletModifiers);
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

	InputComponent->BindAction("Shoot", IE_Pressed, this, &AMainCharacterController::Shoot);

	InputComponent->BindAction("Restart", IE_Pressed, this, &AMainCharacterController::Restart);
}

bool AMainCharacterController::CheckIfDead()
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetCharacter());
	if (!MainCharacter) return true;

	return MainCharacter->GetIsDead();
}