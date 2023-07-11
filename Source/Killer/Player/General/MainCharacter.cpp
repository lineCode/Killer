#include "MainCharacter.h"
#include "AbilitySystemComponent.h"
#include "MainCharacterController.h"
#include "MainCharacterHUD.h"
#include "NiagaraComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Killer/Combat/Health/HealthComponent.h"
#include "Killer/Combat/Weapons/Gun.h"
#include "Killer/Combat/Weapons/WeaponComponent.h"
#include "Killer/Effects/EffectsActor.h"
#include "Killer/General/Save/Save.h"
#include "Killer/Player/Multiplayer/MainCharacterStateMultiplayer.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/PlayerStart.h"
#include "Killer/Combat/AbilitySystem/Attributes/Player/PlayerAttributeSet.h"
#include "Killer/Combat/AbilitySystem/UIData/UpgradeUIData.h"
#include "Killer/UI/HUD/UpgradeNotifyWidget.h"
#include "Killer/Upgrades/Upgrade.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon Component"));
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Ability System Component"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	PlayerAttributes = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("Player Attribute Set"));
}

void AMainCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainCharacter, MainCharacterController);
	DOREPLIFETIME(AMainCharacter, PlayerMaterial);
}

void AMainCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	MainCharacterController = Cast<AMainCharacterController>(NewController);

	WeaponComponent->Server_SpawnWeapon(MainCharacterController);

	Client_ChangePlayerMaterial();
}

void AMainCharacter::UnPossessed()
{
	Super::UnPossessed();

	WeaponComponent->Server_DestroyWeapon();
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		AUpgrade::ApplyGameplayEffectToMainCharacter(this, DefaultAttributeEffect, this);

		GiveAbilities();

		AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &AMainCharacter::OnGameplayEffectAppliedToSelf);
		
		TeleportPlayerToRandomSpawn();
	}

	if (IsLocallyControlled())
	{
		Server_ChangePlayerName(USave::GetSave()->PlayerName);
	}
}

void AMainCharacter::OnGameplayEffectAppliedToSelf(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec,
	FActiveGameplayEffectHandle Handle)
{
	HandleDamageGameplayEffect(Spec);
	HandleHealGameplayEffect(Spec);
	HandleUpgradeGameplayEffect(Spec);
}

void AMainCharacter::HandleDamageGameplayEffect(const FGameplayEffectSpec& Spec)
{
	if (Spec.Def.Get()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Damage"))))
	{
		float Damage = 0.0f;
		for (auto& Attribute : Spec.ModifiedAttributes)
		{
			Damage += FMath::Abs<float>(Attribute.TotalMagnitude);
		}

		AController* InstigatorController = nullptr;
		if (const APawn* InstigatorPawn = Cast<APawn>(Spec.GetContext().GetInstigator()))
		{
			InstigatorController = InstigatorPawn->GetController();
		}
		
		GetHealthComponent()->OnActorTakeAnyDamage(this, Damage, nullptr, InstigatorController, Spec.GetContext().GetEffectCauser());
	}
}

void AMainCharacter::HandleHealGameplayEffect(const FGameplayEffectSpec& Spec) const
{
	if (Spec.Def.Get()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Heal"))))
	{
		float HealAmount = 0.0f;
		for (auto& Attribute : Spec.ModifiedAttributes)
		{
			HealAmount += FMath::Abs<float>(Attribute.TotalMagnitude);
		}
		
		GetHealthComponent()->Server_HealOwner(HealAmount);
	}
}

void AMainCharacter::HandleUpgradeGameplayEffect(const FGameplayEffectSpec& Spec)
{
	if (Spec.Def.Get()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Upgrade"))))
	{
		Client_ShowUpgradeNotification(Cast<UUpgradeUIData>(Spec.Def.Get()->UIData));
	}
}

void AMainCharacter::Client_ShowUpgradeNotification_Implementation(const UUpgradeUIData* UIData)
{
	if (MainCharacterHUD)
	{
		MainCharacterHUD->ShowUpgradeNotification(UIData);
	}
}

void AMainCharacter::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (const auto& Ability : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(Ability, 1, INDEX_NONE, this));
		}
	}
}

void AMainCharacter::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCharacterAnimation();

	RotateCharacter();

	ActivateFootstepsParticles();

	FVector FixedYLocation = GetActorLocation();
	FixedYLocation.Y = 0.0f;
	SetActorLocation(FixedYLocation);
}

void AMainCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (FMath::Abs(GetCharacterMovement()->Velocity.Z) < LandingImpactSpeed)
	{
		return;
	}

	if (MainCharacterController && LandingCameraShakeClass)
	{
		MainCharacterController->PlayerCameraManager->StartCameraShake(LandingCameraShakeClass);
	}

	if (UWorld* World = GetWorld(); LandingEffectsActorClass)
	{
		if (auto* LandingEffectsActor = World->SpawnActor<AEffectsActor>(LandingEffectsActorClass, GetActorLocation(), FRotator::ZeroRotator))
		{
			LandingEffectsActor->Server_SetParticlesMaterial(PlayerMaterial);
		}
	}
}

void AMainCharacter::OnKilled(AController* InstigatedBy, AActor* DamageCauser)
{
	SetActorHiddenInGame(true);
	WeaponComponent->GetGun()->SetActorHiddenInGame(true);

	Multicast_OnKilled();
	Client_OnKilled();

	GetCharacterMovement()->SetMovementMode(MOVE_None);

	if (MainCharacterController)
	{
		if (auto* PlayerStateMultiplayer = MainCharacterController->GetPlayerState<AMainCharacterStateMultiplayer>())
		{
			PlayerStateMultiplayer->Server_IncrementDeathsCount();
		}
	}
}

void AMainCharacter::OnRevived()
{
	SetActorHiddenInGame(false);
	WeaponComponent->GetGun()->SetActorHiddenInGame(false);

	Multicast_OnRevived();
	Client_OnRevived();

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	TeleportPlayerToRandomSpawn();
}

void AMainCharacter::Multicast_OnRevived_Implementation()
{
	GetCapsuleComponent()->SetCollisionProfileName("PlayerAlive");
}

void AMainCharacter::Multicast_OnKilled_Implementation()
{
	GetCapsuleComponent()->SetCollisionProfileName("PlayerDead");
}

void AMainCharacter::Client_OnKilled_Implementation()
{
	if (MainCharacterHUD)
	{
		MainCharacterHUD->ShowRestartTextWidget();
	}
}

void AMainCharacter::Client_OnRevived_Implementation()
{
	if (MainCharacterHUD)
	{
		MainCharacterHUD->HideRestartTextWidget();
	}
}

void AMainCharacter::OnDamageCaused(AActor* DamageCausedTo, const float Damage)
{
	if (!Cast<AMainCharacter>(DamageCausedTo))
	{
		UGameplayStatics::ApplyDamage(this, Damage * SelfDamageMultiplier, GetController(), this, SelfDamageTypeClass);
	}
}

void AMainCharacter::OnKillCaused(AActor* KillCausedTo)
{
	if (MainCharacterController)
	{
		if (auto* PlayerStateMultiplayer = MainCharacterController->GetPlayerState<AMainCharacterStateMultiplayer>())
		{
			PlayerStateMultiplayer->Server_IncrementKillsCount();
		}
	}

	if (USave* Save = USave::GetSave())
	{
		Save->TotalKills++;

		UGameplayStatics::SaveGameToSlot(Save, Save->SlotName, 0);
	}
}

void AMainCharacter::OnRep_PlayerMaterial()
{
	GetSprite()->SetMaterial(0, PlayerMaterial);

	InitializeFootstepsEffects();

	HealthComponent->TryInitializeOwnerDynamicMaterials();
}

void AMainCharacter::Server_ChangePlayerMaterial_Implementation(UMaterialInterface* Material)
{
	PlayerMaterial = Material;

	OnRep_PlayerMaterial();
}

void AMainCharacter::Client_ChangePlayerMaterial_Implementation()
{
	Server_ChangePlayerMaterial(USave::GetSave()->PlayerMaterial);
}

void AMainCharacter::TeleportPlayerToRandomSpawn()
{
	TArray<AActor*> AllSpawns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), AllSpawns);

	if (AllSpawns.Num() > 0)
	{
		const int32 RandomIndex = FMath::RandRange(0, AllSpawns.Num() - 1);
		SetActorLocation(AllSpawns[RandomIndex]->GetActorLocation(), false, nullptr,
		                 ETeleportType::TeleportPhysics);
	}
}

void AMainCharacter::InitializeFootstepsEffects()
{
	FVector WalkParticlesLocation;
	WalkParticlesLocation.Z = -GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FootstepsParticlesComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(WalkParticles, GetCapsuleComponent(), FName(),
	                                                                      WalkParticlesLocation, FRotator::ZeroRotator,
	                                                                      EAttachLocation::KeepRelativeOffset, false,
	                                                                      false);

	FootstepsParticlesComponent->SetVariableMaterial("Material", PlayerMaterial);

	GetWorldTimerManager().SetTimer(FootstepsTimerHandle, this, &AMainCharacter::PlayFootstepsSound,
	                                FootstepsSoundInterval, true, FootstepsSoundInterval);
}

void AMainCharacter::Server_ChangePlayerName_Implementation(const FString& Name)
{
	UGameplayStatics::GetGameMode(this)->ChangeName(GetController(), Name, false);
}

void AMainCharacter::UpdateCharacterAnimation() const
{
	UPaperFlipbook* FinalFlipbook;

	if (GetCharacterMovement()->IsFalling())
	{
		FinalFlipbook = JumpFlipbook;
	}
	else if (FMath::Abs(GetCharacterMovement()->Velocity.X) > 0.0f)
	{
		FinalFlipbook = RunFlipbook;
	}
	else
	{
		FinalFlipbook = IdleFlipbook;
	}

	GetSprite()->SetFlipbook(FinalFlipbook);
}

void AMainCharacter::RotateCharacter() const
{
	FRotator Rotation = GetSprite()->GetComponentRotation();

	GetCharacterMovement()->Velocity.X > 0 ? Rotation.Yaw = 0.0f : Rotation.Yaw = 180.0f;

	GetSprite()->SetWorldRotation(Rotation);
}

void AMainCharacter::ActivateFootstepsParticles() const
{
	if (!FootstepsParticlesComponent)
	{
		return;
	}

	if (FMath::Abs(GetCharacterMovement()->Velocity.X) >= SpeedForWalkParticles && !GetCharacterMovement()->IsFalling())
	{
		FootstepsParticlesComponent->Activate();
	}
	else
	{
		FootstepsParticlesComponent->Deactivate();
	}
}

void AMainCharacter::PlayFootstepsSound() const
{
	if (!GetCharacterMovement()->IsFalling() && FMath::Abs(GetCharacterMovement()->Velocity.X) >= SpeedForWalkParticles)
	{
		const float PitchMultiplier = FMath::RandRange(0.95f, 1.05f);

		UGameplayStatics::PlaySoundAtLocation(this, FootstepsSound, GetActorLocation(),
		                                      FRotator::ZeroRotator, 1.0f, PitchMultiplier);
	}
}
