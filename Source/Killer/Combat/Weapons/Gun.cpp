#include "Gun.h"

#include "Killer/Combat/Projectiles/Projectile.h"
#include "Killer/Effects/EffectsActor.h"
#include "Killer/General/Save/Save.h"
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
}

void AGun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGun, GunMaterial);
}

void AGun::BeginPlay()
{
	Super::BeginPlay();

	if (GetInstigator() && GetInstigator()->IsLocallyControlled())
	{
		Server_SetGunMaterial(USave::GetSave()->PlayerMaterial);
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
