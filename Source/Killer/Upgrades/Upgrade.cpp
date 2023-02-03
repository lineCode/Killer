#include "Upgrade.h"

AUpgrade::AUpgrade()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComponent;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook Component"));
	FlipbookComponent->SetupAttachment(RootComponent);
}

void AUpgrade::BeginPlay()
{
	Super::BeginPlay();
	
	World = GetWorld();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AUpgrade::OnUpgradeBeginOverlap);
}

void AUpgrade::OnUpgradeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	
	if (MainCharacter)
	{
		Activate(MainCharacter);
	}
	else
	{
		AGun* Gun = Cast<AGun>(OtherActor);
		if (Gun)
		{
			Activate(Gun->Owner);
		}
	}
}

void AUpgrade::Activate(AMainCharacter* MainCharacter)
{
	if (!World) return;

	UFunctionLibrary::SpawnParticlesAndSound(World, ActivatedParticles, ActivatedSound, GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f), false);
}