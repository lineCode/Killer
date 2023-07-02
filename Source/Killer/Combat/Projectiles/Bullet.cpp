#include "Bullet.h"
#include "BulletInfo.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	UGameplayStatics::ApplyDamage(OtherActor, BulletInfo.Damage, GetInstigatorController(), this, BulletInfo.DamageTypeClass);

	SpawnBulletDestroyEffects();

	if (const auto* Character = Cast<ACharacter>(OtherActor))
	{
		const FVector ImpulseDirection = (Character->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		Character->GetCharacterMovement()->AddImpulse(ImpulseDirection * BulletInfo.ImpulseForce, true);
	}

	Destroy();
}

void ABullet::ModifyBulletInfo(const FBulletInfo& BulletModifiers)
{
	BulletInfo.Damage = FMath::RandRange(BulletInfo.MinDamage, BulletInfo.MaxDamage) * BulletInfo.DamageMultiplier;
	BulletInfo.Damage *= BulletModifiers.DamageMultiplier;
	
	if (BulletModifiers.DamageTypeClass)
	{
		BulletInfo.DamageTypeClass = BulletModifiers.DamageTypeClass;
	}

	BulletInfo.ImpulseForce *= BulletModifiers.ImpulseForce;
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
