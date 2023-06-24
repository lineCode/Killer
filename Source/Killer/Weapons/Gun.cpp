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

    bCanShoot = true;
}

void AGun::FireFromMuzzle(const FBulletInfo& BulletModifiers)
{
    if (!bCanShoot) return;

    SpawnBullet(BulletModifiers);

    SpawnEffects();

    bCanShoot = false;

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &AGun::ResetFireRate, TimeToShoot, false, TimeToShoot);
}

void AGun::SpawnBullet(const FBulletInfo& BulletModifiers)
{
    UWorld* World = GetWorld();
    if (!World) return;

    FTransform BulletSpawnTransform;
    BulletSpawnTransform.SetLocation(MuzzleLocation->GetComponentLocation());
    BulletSpawnTransform.SetRotation(MuzzleLocation->GetComponentRotation().Quaternion());

    ABullet* Bullet = World->SpawnActorDeferred<ABullet>(BulletClass, BulletSpawnTransform, this, GetInstigator());
    if (!Bullet) return;

    Bullet->SetActorRotation(MuzzleLocation->GetComponentRotation());

    Bullet->ModifyBulletInfo(BulletModifiers);

    UGameplayStatics::FinishSpawningActor(Bullet, BulletSpawnTransform);
}

void AGun::SpawnEffects()
{
    if (MuzzleLocation)
    {
        if (AController* InstigatorController = GetInstigatorController())
        {
            if (const APlayerController* PlayerController = Cast<APlayerController>(InstigatorController))
            {
                GunshotEffectsInfo.PlayerCameraManager = PlayerController->PlayerCameraManager;
            }
        }
        
        GunshotEffectsInfo.Location = MuzzleLocation->GetComponentLocation();
        GunshotEffectsInfo.Rotation = MuzzleLocation->GetComponentRotation();
        
        UFunctionLibrary::ActivateEffects(this, GunshotEffectsInfo);
    }
}

void AGun::ResetFireRate()
{
    bCanShoot = true;
}