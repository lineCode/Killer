#include "Upgrade.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Killer/Combat/Weapons/Gun.h"
#include "Killer/Effects/EffectsActor.h"
#include "Kismet/GameplayStatics.h"

AUpgrade::AUpgrade()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComponent;

	BoxComponent->SetCanEverAffectNavigation(false);
	BoxComponent->SetCollisionProfileName("OverlapAll");

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook Component"));
	FlipbookComponent->SetupAttachment(RootComponent);

	AnimationSpeed = 75.0f;
	AnimationHalfHeightMultiplier = 5.0f;

	bReplicates = true;
	AActor::SetReplicateMovement(true);
}

void AUpgrade::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AnimateUpgrade(DeltaSeconds);
}

FActiveGameplayEffectHandle AUpgrade::ApplyGameplayEffectToMainCharacter(
const AMainCharacter* MainCharacter, const TSubclassOf<UGameplayEffect> EffectClass, const UObject* SourceObject,
	AActor* Instigator, AActor* EffectCauser)
{
	if (!MainCharacter || !EffectClass)
	{
		return {};
	}

	UAbilitySystemComponent* AbilitySystemComponent = MainCharacter->GetAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return {};
	}
	
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(SourceObject);
	EffectContext.AddInstigator(Instigator, EffectCauser);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1, EffectContext);
	if (SpecHandle.IsValid())
	{
		return AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}

	return {};
}

void AUpgrade::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AUpgrade::OnUpgradeBeginOverlap);
	}
}

void AUpgrade::OnUpgradeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	if (AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor))
	{
		ActivateUpgrade(MainCharacter);
	}
	else
	{
		if (const AGun* Gun = Cast<AGun>(OtherActor))
		{
			ActivateUpgrade(Cast<AMainCharacter>(Gun->GetOwner()));
		}
	}
}

void AUpgrade::ActivateUpgrade(AMainCharacter* MainCharacter)
{
	UWorld* World = GetWorld();
	if (!World || !MainCharacter)
	{
		return;
	}

	World->SpawnActor<AEffectsActor>(PickupEffectsActor, GetActorLocation(), GetActorRotation());
}

void AUpgrade::AnimateUpgrade(const float DeltaSeconds)
{
	FVector NewLocation = GetActorLocation();

	NewLocation.Z += FMath::Sin(UGameplayStatics::GetTimeSeconds(this) * AnimationHalfHeightMultiplier) * AnimationSpeed
		* DeltaSeconds;

	SetActorLocation(NewLocation);
}
