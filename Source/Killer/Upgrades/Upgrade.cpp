#include "Upgrade.h"

#include "Killer/Combat/Weapons/Gun.h"
#include "Killer/Effects/EffectsActor.h"
#include "Kismet/GameplayStatics.h"

AUpgrade::AUpgrade()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComponent;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook Component"));
	FlipbookComponent->SetupAttachment(RootComponent);

	AnimationSpeed = 75.0f;
	AnimationHalfHeightMultiplier = 5.0f;

	bReplicates = true;
	AActor::SetReplicateMovement(true);
}

void AUpgrade::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AnimateUpgrade(DeltaSeconds);
}

void AUpgrade::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AUpgrade::OnUpgradeBeginOverlap);
	}
}

void AUpgrade::OnUpgradeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor))
	{
		Activate(MainCharacter);
	}
	else
	{
		if (const AGun* Gun = Cast<AGun>(OtherActor))
		{
			Activate(Cast<AMainCharacter>(Gun->GetOwner()));
		}
	}
}

void AUpgrade::Activate(AMainCharacter* MainCharacter)
{
	UWorld* World = GetWorld();
	if (!World || !MainCharacter)
	{
		return;
	}

	World->SpawnActor<AEffectsActor>(PickupEffectsActor, GetActorLocation(), GetActorRotation());
}

void AUpgrade::AnimateUpgrade(const float DeltaSeconds)
{
	FVector NewLocation = GetActorLocation();

	NewLocation.Z += FMath::Sin(UGameplayStatics::GetTimeSeconds(this) * AnimationHalfHeightMultiplier) * AnimationSpeed
		* DeltaSeconds;

	SetActorLocation(NewLocation);
}
