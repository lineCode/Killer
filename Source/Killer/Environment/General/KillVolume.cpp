#include "KillVolume.h"

#include "GameFramework/PlayerStart.h"
#include "Killer/Combat/Health/HealthComponent.h"
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

	if (HasAuthority())
	{
		if (bReverseKillVolume)
		{
			BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AKillVolume::OnKillVolumeEndOverlap);
		}
		else
		{	
			BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AKillVolume::OnKillVolumeOverlapBegin);
		}
	}
}

void AKillVolume::TeleportActorToRandomPlayerStart(AActor* Actor) const
{
	TArray<AActor*> AllSpawns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), AllSpawns);

	if (AllSpawns.Num() > 0)
	{
		const int32 RandomIndex = FMath::RandRange(0, AllSpawns.Num() - 1);
		
		Actor->SetActorLocation(AllSpawns[RandomIndex]->GetActorLocation(), false, nullptr,
						 ETeleportType::TeleportPhysics);
	}
}

void AKillVolume::KillActor(const AActor* Actor)
{
	if (UActorComponent* ActorHealthComponent = Actor->GetComponentByClass(UHealthComponent::StaticClass()))
	{
		if (auto* HealthComponent = Cast<UHealthComponent>(ActorHealthComponent))
		{
			HealthComponent->Server_KillOwner(nullptr, this);
		}
	}
}

void AKillVolume::TryHandleVolumeOverlap(AActor* Actor)
{
	for (const auto& Class : AffectedClasses)
	{
		if (Actor->IsA(Class))
		{
			if (bShouldKill)
			{
				KillActor(Actor);
			}
			else
			{
				TeleportActorToRandomPlayerStart(Actor);
			}

			return;
		}
	}
}

void AKillVolume::OnKillVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	TryHandleVolumeOverlap(OtherActor);
}

void AKillVolume::OnKillVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TryHandleVolumeOverlap(OtherActor);
}
