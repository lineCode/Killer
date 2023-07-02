#include "Gun.h"
#include "Killer/Effects/EffectsActor.h"
#include "Killer/General/Save/Save.h"
#include "Killer/Projectiles/Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

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

void AGun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGun, GunMaterial);
}

void AGun::BeginPlay()
{
	Super::BeginPlay();

	if (GetInstigator()->IsLocallyControlled())
	{
		Server_SetGunMaterial(USave::GetSave()->PlayerMaterial);
	}
}

void AGun::FireFromMuzzle(const FBulletInfo& BulletModifiers)
{
	if (!bCanShoot)
	{
		return;
	}

	Server_SpawnBullet(BulletModifiers);

	Server_SpawnGunshotEffects();

	StartGunshotCameraShake();

	bCanShoot = false;

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &AGun::ResetFireRate, TimeToShoot, false, TimeToShoot);
}

void AGun::Server_SpawnBullet_Implementation(const FBulletInfo& BulletModifiers)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FTransform BulletSpawnTransform;
	BulletSpawnTransform.SetLocation(MuzzleLocation->GetComponentLocation());
	BulletSpawnTransform.SetRotation(MuzzleLocation->GetComponentRotation().Quaternion());

	ABullet* Bullet = World->SpawnActorDeferred<ABullet>(BulletClass, BulletSpawnTransform, this, GetInstigator());
	if (!Bullet)
	{
		return;
	}

	Bullet->SetActorRotation(MuzzleLocation->GetComponentRotation());

	Bullet->ModifyBulletInfo(BulletModifiers);

	UGameplayStatics::FinishSpawningActor(Bullet, BulletSpawnTransform);
}

void AGun::Server_SpawnGunshotEffects_Implementation()
{
	if (UWorld* World = GetWorld(); GunshotEffectsActor)
	{
		World->SpawnActor<AEffectsActor>(GunshotEffectsActor, MuzzleLocation->GetComponentLocation(),
		                                 MuzzleLocation->GetComponentRotation());
	}
}

void AGun::StartGunshotCameraShake() const
{
	if (GunshotCameraShakeClass)
	{
		if (AController* InstigatorController = GetInstigatorController())
		{
			if (const APlayerController* PlayerController = Cast<APlayerController>(InstigatorController))
			{
				PlayerController->PlayerCameraManager->StartCameraShake(GunshotCameraShakeClass);
			}
		}
	}
}

void AGun::Server_SetGunMaterial_Implementation(UMaterialInterface* Material)
{
	GunMaterial = Material;

	OnRep_GunMaterial();
}

void AGun::OnRep_GunMaterial()
{
	GetSprite()->SetMaterial(0, GunMaterial);
}

void AGun::ResetFireRate()
{
	bCanShoot = true;
}
