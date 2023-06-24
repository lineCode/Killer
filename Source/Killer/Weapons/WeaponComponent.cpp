#include "WeaponComponent.h"
#include "Gun.h"
#include "Killer/General/Save.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	
	if (Owner)
	{
		if (const APawn* OwnerPawn = Cast<APawn>(Owner))
		{
			OwnerController = Cast<APlayerController>(OwnerPawn->GetController());
		}
	}
	
	if (GetOwnerRole() == ROLE_Authority)
	{
		SpawnWeaponMulticast();
	}
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveWeapon();
	RotateWeapon();
}

void UWeaponComponent::MoveWeapon()
{
	if (!OwnerController)
	{
		return;
	}

	FVector CursorLocation;
	FVector CursorDirection;
	OwnerController->DeprojectMousePositionToWorld(CursorLocation, CursorDirection);

	const FVector CursorWorldLocation = CursorLocation.Y / CursorDirection.Y * -CursorDirection + CursorLocation;

	MoveWeaponMulticast(CursorWorldLocation);
}

void UWeaponComponent::RotateWeapon()
{
	if (!Gun || !Owner)
	{
		return;
	}

	FVector Direction = Gun->GetActorLocation() - Owner->GetActorLocation();
	Direction.Y = 0.0f;

	const FQuat GunRotation = FQuat(Direction.Rotation());

	RotateWeaponMulticast(GunRotation);
}

void UWeaponComponent::SpawnWeaponMulticast_Implementation()
{
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
	
		if (OwnerController)
		{
			GunSpawnParameters.Instigator = OwnerController->GetPawn();
		}
	
		GunSpawnParameters.Owner = Owner;
		
		Gun = World->SpawnActor<AGun>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, GunSpawnParameters);
	}
}

void UWeaponComponent::MoveWeaponMulticast_Implementation(const FVector& Location)
{
	if (!Gun)
	{
		return;
	}

	Gun->SetActorLocation(Location, true);
}

void UWeaponComponent::RotateWeaponMulticast_Implementation(const FQuat& Rotation)
{
	if (!Gun)
	{
		return;
	}

	Gun->SetActorRotation(Rotation);
}
