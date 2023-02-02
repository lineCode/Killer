#include "Target.h"
#include "Kismet/GameplayStatics.h"
#include "Killer/Combat/DamageNumbers.h"
#include "Killer/Player/MainCharacter.h"

ATarget::ATarget()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	RootComponent = CapsuleComponent;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook Component"));
	FlipbookComponent->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

void ATarget::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();

	//SetLocations();
}

void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsMovingToFirstLocation)
	{
		MoveToLocation(SecondLocation);
	}
	else
	{
		MoveToLocation(FirstLocation);
	}
}

void ATarget::SetHalfPatroulDistance(float Value)
{
	if (Value < 0.0f) return;

	HalfPatroulDistance = Value;

	SetLocations();
}

void ATarget::SetLocations()
{
	FirstLocation = GetActorLocation();
	SecondLocation = GetActorLocation();

	FirstLocation.X -= HalfPatroulDistance;
	SecondLocation.X += HalfPatroulDistance;

	SetActorLocation(FirstLocation);
}

void ATarget::OnKilled(AController* InstigatedBy, AActor* DamageCauser)
{
	if (World)
	{
		TArray<AActor*> ActorsWithInterface;
		UGameplayStatics::GetAllActorsWithInterface(World, UTargetEventsInterface::StaticClass(), ActorsWithInterface);

		for (auto& Actor : ActorsWithInterface)
		{
			ITargetEventsInterface* InterfaceActor = Cast<ITargetEventsInterface>(Actor);
			if (InterfaceActor)
			{
				InterfaceActor->OnTargetKilled(InstigatedBy, DamageCauser);
			}
		}
	}

	Destroy();
}

void ATarget::MoveToLocation(const FVector& Location)
{
	FVector Direction = (Location - GetActorLocation()).GetSafeNormal();
	FVector NewLocation = GetActorLocation() + Direction * Speed;
	SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

	if (!IsMovingToFirstLocation && GetActorLocation().Equals(SecondLocation, AcceptanceRadius))
	{
		IsMovingToFirstLocation = true;
	}
	if (IsMovingToFirstLocation && GetActorLocation().Equals(FirstLocation, AcceptanceRadius))
	{
		IsMovingToFirstLocation = false;
	}
}