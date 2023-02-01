#include "Gun.h"
#include "Killer/Projectiles/Bullet.h"
#include "Kismet/GameplayStatics.h"

AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComponent;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook Component"));
	FlipbookComponent->SetupAttachment(RootComponent);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	MuzzleLocation->SetupAttachment(RootComponent);
}

void AGun::FireFromMuzzle(AController* InstigatedBy)
{
	UWorld* World = GetWorld();
	if (!World) return;

	ABullet* Bullet = World->SpawnActor<ABullet>(BulletClass, MuzzleLocation->GetComponentLocation(), MuzzleLocation->GetComponentRotation());
	if (!Bullet) return;

	Bullet->FireInDirection(MuzzleLocation->GetForwardVector(), InstigatedBy);

	UFunctionLibrary::SpawnParticlesAndSound(World, ShootParticles, ShootSound, MuzzleLocation->GetComponentLocation(), MuzzleLocation->GetComponentRotation());
}

UPaperFlipbookComponent* AGun::GetSprite()
{
	return FlipbookComponent;
}
