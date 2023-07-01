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
	DOREPLIFETIME_CONDITION(UWeaponComponent, MainCharacterOwner, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UWeaponComponent, MainCharacterController, COND_OwnerOnly);
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		MainCharacterOwner = Cast<AMainCharacter>(GetOwner());
	}
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MainCharacterController && MainCharacterController->IsLocalController())
	{
		MoveWeapon();
	}

	if (GetOwnerRole() == ROLE_Authority)
	{
		RotateWeapon();
	}
}

void UWeaponComponent::Server_SpawnWeapon_Implementation(AMainCharacterController* Controller)
{
	MainCharacterController = Controller;

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

		GunSpawnParameters.Instigator = MainCharacterOwner;
		GunSpawnParameters.Owner = MainCharacterOwner;

		Gun = World->SpawnActor<AGun>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, GunSpawnParameters);
	}
}

void UWeaponComponent::Server_DestroyWeapon_Implementation()
{
	Gun->Destroy();
}

void UWeaponComponent::MoveWeapon() const
{
	if (!MainCharacterOwner || !MainCharacterController)
	{
		return;
	}

	FVector CursorLocation;
	FVector CursorDirection;
	MainCharacterController->DeprojectMousePositionToWorld(CursorLocation, CursorDirection);

	const FVector CursorWorldLocation = CursorLocation.Y / CursorDirection.Y * -CursorDirection + CursorLocation;

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
	if (!Gun || !MainCharacterOwner)
	{
		return;
	}

	FVector Direction = Gun->GetActorLocation() - MainCharacterOwner->GetActorLocation();
	Direction.Y = 0.0f;

	const FQuat GunRotation = FQuat(Direction.Rotation());

	Gun->SetActorRotation(GunRotation);
}
