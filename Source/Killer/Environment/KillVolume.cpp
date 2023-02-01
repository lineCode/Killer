#include "KillVolume.h"
#include "Killer/Combat/HealthInterface.h"
#include "Kismet/GameplayStatics.h"

AKillVolume::AKillVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComponent;
}

void AKillVolume::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AKillVolume::OnVolumeOverlapbegin);
}

void AKillVolume::OnVolumeOverlapbegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IHealthInterface* HealthInterfaceActor = Cast<IHealthInterface>(OtherActor);
	if (HealthInterfaceActor)
	{
		HealthInterfaceActor->OnKilled(UGameplayStatics::GetPlayerController(GetWorld(), 0), this);
	}
	else
	{
		OtherActor->Destroy();
	}
}
