#include "KillVolume.h"

#include "Killer/Combat/HealthComponent.h"
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

    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AKillVolume::OnKillVolumeOverlapBegin);
}

void AKillVolume::OnKillVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
    if (UActorComponent* ActorHealthComponent = OtherActor->GetComponentByClass(UHealthComponent::StaticClass()))
    {
        if (UHealthComponent* HealthComponent = Cast<UHealthComponent>(ActorHealthComponent))
        {
            HealthComponent->KillOwner(nullptr, this);

            return;
        }
    }
    
    OtherActor->Destroy();
}
