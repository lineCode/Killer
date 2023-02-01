#include "Bullet.h"
#include "Killer/Combat/HealthInterface.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;

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
	float Damage = FMath::RandRange(MinDamage, MaxDamage);

	UGameplayStatics::ApplyDamage(OtherActor, Damage, Instigator, this, DamageTypeClass);

	UFunctionLibrary::SpawnParticlesAndSound(World, HitParticles, HitSound, GetActorLocation());

	Destroy();
}

void ABullet::OnBulletOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IHealthInterface* HealthInterfaceActor = Cast<IHealthInterface>(OtherActor);
	if (!HealthInterfaceActor) return;

	float Damage = FMath::RandRange(MinDamage, MaxDamage);

	UGameplayStatics::ApplyDamage(OtherActor, Damage, Instigator, this, DamageTypeClass);

	Destroy();
}

void ABullet::FireInDirection(const FVector& Direction, AController* InstigatedBy)
{
	Instigator = InstigatedBy;

	ProjectileMovementComponent->Velocity = Direction.GetSafeNormal() * ProjectileMovementComponent->InitialSpeed;
}