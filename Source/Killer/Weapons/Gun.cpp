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

void AGun::BeginPlay()
{
	Super::BeginPlay();

	CurrentTimeToShoot = TimeToShoot;
}

void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTimeToShoot = FMath::Clamp(CurrentTimeToShoot - DeltaTime, 0.0f, TimeToShoot);
}

void AGun::FireFromMuzzle(FBulletInfo& BulletModifiers)
{
	if (CurrentTimeToShoot > 0.0f) return;
	CurrentTimeToShoot = TimeToShoot;

	UWorld* World = GetWorld();
	if (!World) return;

	ABullet* Bullet = World->SpawnActor<ABullet>(BulletClass, MuzzleLocation->GetComponentLocation(), MuzzleLocation->GetComponentRotation());
	if (!Bullet) return;

	BulletModifiers.StartDirection = MuzzleLocation->GetForwardVector();
	Bullet->FireInDirection(BulletModifiers);

	UFunctionLibrary::SpawnParticlesAndSound(World, ShootParticles, ShootSound, MuzzleLocation->GetComponentLocation(), MuzzleLocation->GetComponentRotation());
}

UPaperFlipbookComponent* AGun::GetSprite()
{
	return FlipbookComponent;
}
