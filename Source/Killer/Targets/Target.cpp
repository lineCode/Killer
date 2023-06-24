#include "Target.h"
#include "Killer/Player/MainCharacter.h"

ATarget::ATarget()
{
    PrimaryActorTick.bCanEverTick = true;

    IsPlacedInWorld = false;

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

    if (IsPlacedInWorld)
    {
        SetLocations();
    }
}

void ATarget::Tick(const float DeltaTime)
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

void ATarget::OnDamageTaken(AController* InstigatedBy, AActor* DamageCauser)
{
    if (InstigatedBy)
    {
        if (const APlayerController* PlayerController = Cast<APlayerController>(InstigatedBy))
        {
            PlayerController->PlayerCameraManager->StartCameraShake(DamageCameraShakeClass);
        }
    }
}

void ATarget::SetHalfPatrolDistance(const float Value)
{
    if (Value < 0.0f) return;

    HalfPatrolDistance = Value;

    SetLocations();
}

void ATarget::SetLocations()
{
    FirstLocation = GetActorLocation();
    SecondLocation = GetActorLocation();

    FirstLocation.X -= HalfPatrolDistance;
    SecondLocation.X += HalfPatrolDistance;

    SetActorLocation(FirstLocation);
}

void ATarget::OnKilled(AController* InstigatedBy, AActor* DamageCauser)
{
    if (InstigatedBy)
    {
        if (const APlayerController* PlayerController = Cast<APlayerController>(InstigatedBy))
        {
            PlayerController->PlayerCameraManager->StartCameraFade(0.01f, 0.0f, 0.5f, FLinearColor::White);
        }
    }
    
    OnTargetKilled.Broadcast(InstigatedBy, DamageCauser);

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
