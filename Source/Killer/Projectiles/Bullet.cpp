#include "Bullet.h"
#include "Killer/Combat/HealthInterface.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;

	IsInitialized = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComponent;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook Component"));
	FlipbookComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();

	BoxComponent->OnComponentHit.AddDynamic(this, &ABullet::OnBulletHit);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlapBegin);
}

void ABullet::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!IsInitialized) return;

	UGameplayStatics::ApplyDamage(OtherActor, Damage, BulletInfoModifiers.InstigatedBy, this, DamageTypeClass);

	if (World)
	{
		World->SpawnActor<AParticlesAndSound>(HitEffects, GetActorLocation(), GetActorRotation());
	}

	Destroy();
}

void ABullet::OnBulletOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsInitialized) return;

	IHealthInterface* HealthInterfaceActor = Cast<IHealthInterface>(OtherActor);
	if (!HealthInterfaceActor) return;

	UGameplayStatics::ApplyDamage(OtherActor, Damage, BulletInfoModifiers.InstigatedBy, this, DamageTypeClass);

	Destroy();
}

void ABullet::FireInDirection(const FBulletInfo& BulletModifiers)
{
	ModifyBulletInfo(BulletModifiers);
}

void ABullet::ModifyBulletInfo(const FBulletInfo& BulletModifiers)
{
	BulletInfoModifiers.InstigatedBy = BulletModifiers.InstigatedBy;

	Damage = FMath::RandRange(MinDamage, MaxDamage);
	Damage *= BulletModifiers.DamageMultiplier;

	IsInitialized = true;
}