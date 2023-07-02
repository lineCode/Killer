#include "MainCharacterController.h"
#include "GameFramework/Character.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Killer/Effects/EffectsActor.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MainCharacterHUD.h"
#include "Killer/Combat/Health/HealthComponent.h"
#include "Killer/Combat/Weapons/Gun.h"
#include "Killer/Combat/Weapons/WeaponComponent.h"
#include "Killer/Input/InputActionsData.h"
#include "Net/UnrealNetwork.h"

AMainCharacterController::AMainCharacterController()
{
	CanShoot = true;
}

void AMainCharacterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (HasAuthority())
	{
		MainCharacter = Cast<AMainCharacter>(InPawn);
	}
}

void AMainCharacterController::BeginPlay()
{
	Super::BeginPlay();

	const FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
	SetShowMouseCursor(false);
}

void AMainCharacterController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainCharacterController, MainCharacter);
}

void AMainCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (EnhancedInputComponent && InputActionsData)
	{
		EnhancedInputComponent->BindAction(InputActionsData->MoveInput, ETriggerEvent::Triggered, this,
		                                   &AMainCharacterController::Move);
		EnhancedInputComponent->BindAction(InputActionsData->ShootInput, ETriggerEvent::Triggered, this,
		                                   &AMainCharacterController::Shoot);
		EnhancedInputComponent->BindAction(InputActionsData->JumpInput, ETriggerEvent::Started, this,
		                                   &AMainCharacterController::Jump);
		EnhancedInputComponent->BindAction(InputActionsData->JumpInput, ETriggerEvent::Completed, this,
		                                   &AMainCharacterController::StopJumping);
		EnhancedInputComponent->BindAction(InputActionsData->JumpInput, ETriggerEvent::Canceled, this,
		                                   &AMainCharacterController::StopJumping);
		EnhancedInputComponent->BindAction(InputActionsData->PauseInput, ETriggerEvent::Triggered, this,
		                                   &AMainCharacterController::PauseGame);
		EnhancedInputComponent->BindAction(InputActionsData->RestartInput, ETriggerEvent::Triggered, this,
		                                   &AMainCharacterController::Restart);
	}
}

bool AMainCharacterController::CanProceedInput() const
{
	return MainCharacter && !MainCharacter->GetHealthComponent()->IsDead() && bIsInputEnabled;
}

void AMainCharacterController::Move(const FInputActionValue& Value)
{
	if (!CanProceedInput())
	{
		return;
	}

	const FVector2D MoveValue = Value.Get<FVector2D>();

	MainCharacter->AddMovementInput(FVector(1, 0, 0), MoveValue.Y);
	MainCharacter->AddMovementInput(FVector(1, 0, 0), MoveValue.X);
}

void AMainCharacterController::Jump(const FInputActionValue& Value)
{
	if (!CanProceedInput())
	{
		return;
	}

	ACharacter* PossessedCharacter = GetCharacter();
	if (!PossessedCharacter)
	{
		return;
	}

	if (PossessedCharacter->CanJump())
	{
		PossessedCharacter->Jump();

		Server_SpawnJumpEffects();
	}
}

void AMainCharacterController::StopJumping(const FInputActionValue& Value)
{
	if (!CanProceedInput())
	{
		return;
	}

	ACharacter* PossessedCharacter = GetCharacter();
	if (!PossessedCharacter)
	{
		return;
	}

	PossessedCharacter->StopJumping();
}

void AMainCharacterController::Server_SpawnJumpEffects_Implementation()
{
	const ACharacter* PossessedCharacter = GetCharacter();

	UWorld* World = GetWorld();

	if (!PossessedCharacter || !World || !JumpEffectsActorClass)
	{
		return;
	}

	FVector JumpEffectsLocation = PossessedCharacter->GetActorLocation();
	JumpEffectsLocation.Z -= PossessedCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	if (auto* JumpEffectsActor = World->SpawnActor<AEffectsActor>(JumpEffectsActorClass, JumpEffectsLocation, FRotator::ZeroRotator); MainCharacter)
	{
		JumpEffectsActor->Server_SetParticlesMaterial(MainCharacter->GetPlayerMaterial());
	}
}

void AMainCharacterController::Shoot(const FInputActionValue& Value)
{
	if (!CanProceedInput())
	{
		return;
	}

	const UWeaponComponent* WeaponComponent = MainCharacter->GetWeaponComponent();
	if (!WeaponComponent)
	{
		return;
	}

	AGun* Gun = WeaponComponent->GetGun();
	if (!Gun)
	{
		return;
	}

	if (Gun->IsAutomatic() && Value.Get<bool>())
	{
		Gun->FireFromMuzzle(MainCharacter->BulletModifiers);
	}
	else if (!Gun->IsAutomatic() && Value.Get<bool>() && CanShoot)
	{
		Gun->FireFromMuzzle(MainCharacter->BulletModifiers);

		CanShoot = false;
	}
	else if (!Value.Get<bool>())
	{
		CanShoot = true;
	}
}

void AMainCharacterController::PauseGame(const FInputActionValue& Value)
{
	if (UGameplayStatics::IsGamePaused(this))
	{
		UnPause();
	}
	else
	{
		Pause();
	}
}

void AMainCharacterController::Pause()
{
	UGameplayStatics::SetGamePaused(this, true);

	const FInputModeGameAndUI InputModeGameAndUI;
	SetInputMode(InputModeGameAndUI);

	SetShowMouseCursor(true);

	if (MainCharacter)
	{
		MainCharacter->GetMainCharacterHUD()->ShowPauseWidget();
	}
}

void AMainCharacterController::UnPause()
{
	UGameplayStatics::SetGamePaused(this, false);

	const FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);

	SetShowMouseCursor(false);

	if (MainCharacter)
	{
		MainCharacter->GetMainCharacterHUD()->HidePauseWidget();
	}
}

void AMainCharacterController::Restart(const FInputActionValue& Value)
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
