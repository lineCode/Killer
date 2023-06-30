#include "MainCharacter.h"
#include "MainCharacterController.h"
#include "MainCharacterHUD.h"
#include "NiagaraComponent.h"
#include "PaperFlipbookComponent.h"
#include "PlayerStateMultiplayer.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Killer/Environment/ObjectSpawn.h"
#include "Killer/Effects/EffectsActor.h"
#include "Killer/General/Save.h"
#include "Killer/UI/HUD/HUDWidget.h"
#include "Killer/Weapons/Gun.h"
#include "Killer/Weapons/WeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

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
}

void AMainCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMainCharacter, MainCharacterController);
}

void AMainCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    MainCharacterController = Cast<AMainCharacterController>(NewController);

    WeaponComponent->SetMainCharacterController(MainCharacterController);
}

void AMainCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    if (HasAuthority())
    {
        TeleportPlayerToRandomSpawn();
    }

    InitializeFootstepsEffects();
}

void AMainCharacter::Tick(const float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    UpdateCharacterAnimation();
    
    RotateCharacter();

    ActivateWalkParticles();
    
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

    if (MainCharacterController && LandingCameraShake)
    {
        MainCharacterController->PlayerCameraManager->StartCameraShake(LandingCameraShake);
    }

    if (UWorld* World = GetWorld(); LandingEffectsActor)
    {
        World->SpawnActor<AEffectsActor>(LandingEffectsActor, GetActorLocation(), FRotator::ZeroRotator);
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
        if (auto* PlayerStateMultiplayer = MainCharacterController->GetPlayerState<APlayerStateMultiplayer>())
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
        MainCharacterHUD->GetHUDWidget()->ShowDeathText();
    }
}

void AMainCharacter::Client_OnRevived_Implementation()
{
    if (MainCharacterHUD)
    {
        MainCharacterHUD->GetHUDWidget()->HideDeathText();
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
        if (auto* PlayerStateMultiplayer = MainCharacterController->GetPlayerState<APlayerStateMultiplayer>())
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

void AMainCharacter::TeleportPlayerToRandomSpawn()
{
    TArray<AActor*> AllSpawns;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AObjectSpawn::StaticClass(), AllSpawns);

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
    
    WalkParticlesComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(WalkParticles, GetCapsuleComponent(), FName(),
        WalkParticlesLocation, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, false, false);

    GetWorldTimerManager().SetTimer(FootstepsTimerHandle, this, &AMainCharacter::PlayFootstepsSound,
        FootstepsSoundInterval, true, FootstepsSoundInterval);
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

void AMainCharacter::ActivateWalkParticles() const
{
    if (!WalkParticlesComponent)
    {
        return;
    }
    
    if (FMath::Abs(GetCharacterMovement()->Velocity.X) >= SpeedForWalkParticles && !GetCharacterMovement()->IsFalling())
    {
        WalkParticlesComponent->Activate();
    }
    else
    {
        WalkParticlesComponent->Deactivate();
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