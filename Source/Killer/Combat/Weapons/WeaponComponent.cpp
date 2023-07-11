#include "WeaponComponent.h"
#include "Gun.h"
#include "Killer/General/Save/Save.h"
#include "Killer/Player/General/MainCharacter.h"
#include "Killer/Player/General/MainCharacterController.h"
#include "Net/UnrealNetwork.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UWeaponComponent, Gun, COND_OwnerOnly);
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		OwnerController = Cast<APlayerController>(OwnerPawn->GetController());
	}
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveWeapon();
	RotateWeapon();
}

void UWeaponComponent::Server_SpawnWeapon_Implementation(APlayerController* Controller)
{
	OwnerController = Controller;

	if (bLoadFromSave)
	{
		if (const USave* Save = USave::GetSave())
		{
			WeaponClass = Save->WeaponClass;
		}
	}

	if (UWorld* World = GetWorld(); WeaponClass)
	{
		FActorSpawnParameters GunSpawnParameters;

		GunSpawnParameters.Instigator = Controller->GetPawn();
		GunSpawnParameters.Owner = Controller->GetPawn();

		Gun = World->SpawnActor<AGun>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, GunSpawnParameters);
	}
}

void UWeaponComponent::Server_DestroyWeapon_Implementation()
{
	Gun->Destroy();
}

void UWeaponComponent::MoveWeapon() const
{
	if (!OwnerController || !Gun)
	{
		return;
	}

	if (!OwnerController->IsLocalController())
	{
		return;
	}

	FVector CursorLocation;
	FVector CursorDirection;
	OwnerController->DeprojectMousePositionToWorld(CursorLocation, CursorDirection);

	const FVector CursorWorldLocation = CursorLocation.Y / CursorDirection.Y * -CursorDirection + CursorLocation;

	// We move weapon on the client and server to exclude movement lags on client.
	Gun->SetActorLocation(CursorWorldLocation, true);

	Server_MoveWeapon(CursorWorldLocation);
}

void UWeaponComponent::Server_MoveWeapon_Implementation(const FVector& Location) const
{
	if (!Gun)
	{
		return;
	}

	Gun->SetActorLocation(Location, true);
}

void UWeaponComponent::RotateWeapon() const
{
	if (!Gun)
	{
		return;
	}

	FVector Direction = Gun->GetActorLocation() - GetOwner()->GetActorLocation();
	Direction.Y = 0.0f;

	const FQuat GunRotation = FQuat(Direction.Rotation());

	Gun->SetActorRotation(GunRotation);
}
