#include "Bullet.h"
#include "Killer/Effects/EffectsActor.h"
#include "Kismet/GameplayStatics.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComponent;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook Component"));
	FlipbookComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("Projectile Movement Component"));
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		BoxComponent->OnComponentHit.AddDynamic(this, &ABullet::OnBulletHit);
	}
}

void ABullet::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                          FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, DamageTypeClass);

	SpawnBulletDestroyEffects();

	Destroy();
}

void ABullet::ModifyBulletInfo(const FBulletInfo& BulletModifiers)
{
	Damage = FMath::RandRange(MinDamage, MaxDamage);
	Damage *= BulletModifiers.DamageMultiplier;
}

void ABullet::SpawnBulletDestroyEffects() const
{
	UWorld* World = GetWorld();
	if (!DestroyEffectsActor || !World)
	{
		return;
	}

	World->SpawnActor<AEffectsActor>(DestroyEffectsActor, GetActorLocation(), GetActorRotation());
}
