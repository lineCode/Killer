#include "MainCharacter.h"
#include "Killer/Weapons/Gun.h"
#include "Killer/Environment/Spawn.h"

AMainCharacter::AMainCharacter()
{
	PlayerController = nullptr;
	Gun = nullptr;
	IsDead = false;
	PlayerDynamicMaterial = nullptr;
	WeaponDynamicMaterial = nullptr;
	PlayerEmission = 0.0f;
	WeaponEmission = 0.0f;
	Kills = 0;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(RootComponent);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComp->SetupAttachment(RootComponent);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();

	TeleportPlayerToRandomSpawn();

	AController* BaseController = GetController();
	if (BaseController)
	{
		PlayerController = Cast<APlayerController>(BaseController);
	}

	if (Weapon)
	{
		Gun = Cast<AGun>(Weapon->GetChildActor());

		if (Gun)
		{
			GetMaterialEmission(Gun->GetSprite(), WeaponDynamicMaterial, WeaponEmission);

			Gun->Owner = this;
		}
	}

	GetMaterialEmission(GetSprite(), PlayerDynamicMaterial, PlayerEmission);

	BulletModifiers.InstigatedBy = GetController();
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead) return;

	MoveWeapon();
	RotateWeapon();
}

void AMainCharacter::OnDamageTaken(AController* InstigatedBy, AActor* DamageCauser)
{
	UpdateMaterialEmission(PlayerDynamicMaterial, PlayerEmission);
	UpdateMaterialEmission(WeaponDynamicMaterial, WeaponEmission);
}

void AMainCharacter::OnKilled(AController* InstigatedBy, AActor* DamageCauser)
{
	IsDead = true;

	SetActorHiddenInGame(true);

	_OnKilled(InstigatedBy, DamageCauser);
}

void AMainCharacter::OnHealed(float HealAmount)
{
	UpdateMaterialEmission(PlayerDynamicMaterial, PlayerEmission);
	UpdateMaterialEmission(WeaponDynamicMaterial, WeaponEmission);
}

void AMainCharacter::OnTargetKilled(AController* InstigatedBy, AActor* DamageCauser)
{
	Kills++;
}

void AMainCharacter::UpdateMaterialEmission(UMaterialInstanceDynamic* DynamicMaterial, float Emission)
{
	if (!HealthComp || !DynamicMaterial || Emission < 0.0f) return;

	DynamicMaterial->SetScalarParameterValue("Emission", Emission * HealthComp->GetHealthPercent());
}

void AMainCharacter::GetMaterialEmission(UPaperFlipbookComponent* FlipbookSprite, UMaterialInstanceDynamic*& DynamicMaterial, float& Emission)
{
	if (!FlipbookSprite) return;

	DynamicMaterial = FlipbookSprite->CreateDynamicMaterialInstance(0);
	if (!DynamicMaterial) return;

	FMaterialParameterInfo MaterialInfo;
	MaterialInfo.Name = "Emission";
	DynamicMaterial->GetScalarParameterValue(MaterialInfo, Emission);
}

void AMainCharacter::MoveWeapon()
{
	if (!PlayerController) return;

	FVector CursorLocation;
	FVector CursorDirection;
	PlayerController->DeprojectMousePositionToWorld(CursorLocation, CursorDirection);

	FVector CursorWorldLocation = CursorLocation.Y / CursorDirection.Y * -CursorDirection + CursorLocation;

	Weapon->SetWorldLocation(CursorWorldLocation, true);

	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Transparent, CursorWorldLocation.ToString());
}

void AMainCharacter::RotateWeapon()
{
	if (!Gun) return;

	FVector Direction = Weapon->GetComponentLocation() - GetActorLocation();
	Direction.Y = 0.0f;

	FQuat WeaponRotation = FQuat(Direction.Rotation());
	Weapon->SetWorldRotation(WeaponRotation);
}

void AMainCharacter::TeleportPlayerToRandomSpawn()
{
	if (World)
	{
		TArray<AActor*> AllSpawns;
		UGameplayStatics::GetAllActorsOfClass(World, ASpawn::StaticClass(), AllSpawns);

		if (AllSpawns.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, AllSpawns.Num() - 1);
			SetActorLocation(AllSpawns[RandomIndex]->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);
		}
	}
}

AGun* AMainCharacter::GetGun()
{
	return Gun;
}

UHealthComponent* AMainCharacter::GetHealthComponent()
{
	return HealthComp;
}

bool AMainCharacter::GetIsDead()
{
	return IsDead;
}