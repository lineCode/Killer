#include "Bullet.h"
#include "Killer/Combat/HealthInterface.h"
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

    World = GetWorld();

    BoxComponent->OnComponentHit.AddDynamic(this, &ABullet::OnBulletHit);
    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlapBegin);
}

void ABullet::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                          FVector NormalImpulse, const FHitResult& Hit)
{
    UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, DamageTypeClass);

    if (Owner)
    {
        HitEffectsInfo.Location = GetActorLocation();
        HitEffectsInfo.Rotation = GetActorRotation();
        
        UFunctionLibrary::ActivateEffects(this, HitEffectsInfo);
    }

    Destroy();
}

void ABullet::OnBulletOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
    if (const IHealthInterface* HealthInterfaceActor = Cast<IHealthInterface>(OtherActor); !HealthInterfaceActor) return;

    UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, DamageTypeClass);

    Destroy();
}

void ABullet::ModifyBulletInfo(const FBulletInfo& BulletModifiers)
{
    Damage = FMath::RandRange(MinDamage, MaxDamage);
    Damage *= BulletModifiers.DamageMultiplier;
}
