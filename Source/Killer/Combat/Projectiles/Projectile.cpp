#include "Projectile.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Killer/Combat/AbilitySystem/Attributes/Player/PlayerAttributeSet.h"
#include "Killer/Effects/EffectsActor.h"
#include "Killer/Player/General/MainCharacter.h"
#include "Killer/Upgrades/Upgrade.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComponent;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook Component"));
	FlipbookComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("Projectile Movement Component"));
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		BoxComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnProjectileHit);
	}
}

void AProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                          FVector NormalImpulse, const FHitResult& Hit)
{
	if (const AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetInstigator()))
	{
		AUpgrade::ApplyGameplayEffectToMainCharacter(Cast<AMainCharacter>(OtherActor), DamageGameplayEffectClass, this,
			GetInstigator(), this);

		if (const auto* Character = Cast<ACharacter>(OtherActor))
		{
			const FVector ImpulseDirection = (Character->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			const float ImpulseForce = MainCharacter->GetPlayerAttributes()->GetProjectileImpulseForce();
		
			Character->GetCharacterMovement()->AddImpulse(ImpulseDirection * ImpulseForce, true);
		}
	}

	SpawnProjectileDestroyEffects();
	Destroy();
}

void AProjectile::SpawnProjectileDestroyEffects() const
{
	UWorld* World = GetWorld();
	if (!DestroyEffectsActor || !World)
	{
		return;
	}

	World->SpawnActor<AEffectsActor>(DestroyEffectsActor, GetActorLocation(), GetActorRotation());
}
