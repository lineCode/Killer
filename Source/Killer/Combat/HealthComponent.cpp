#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Killer/Combat/DamageNumbers.h"
#include "Killer/Player/MainCharacter.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	World = GetWorld();

	Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnActorTakeAnyDamage);

		HealthInterfaceOwner = Cast<IHealthInterface>(Owner);
	}

	MaxHealth = FMath::RandRange(MaxMinHealth, MaxMaxHealth);
	CurrentHealth = MaxHealth;
}

void UHealthComponent::OnActorTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || !Owner || !World) return;

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);

	SpawnNumbers(DamageNumbersClass, Damage);

	UFunctionLibrary::SpawnParticlesAndSound(World, BloodParticles, DamageSound, Owner->GetActorLocation());

	if (HealthInterfaceOwner)
	{
		HealthInterfaceOwner->OnDamageTaken(InstigatedBy, DamageCauser);
	}

	if (!Cast<AMainCharacter>(Owner) && InstigatedBy)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(InstigatedBy->GetCharacter());
		if (MainCharacter)
		{
			UGameplayStatics::ApplyDamage(MainCharacter, Damage, InstigatedBy, DamageCauser, ReturnToPlayerDamageType);
		}
	}

	if (CurrentHealth <= 0.0f)
	{
		if (HealthInterfaceOwner)
		{
			HealthInterfaceOwner->OnKilled(InstigatedBy, DamageCauser);
		}
	}
}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f) return;

	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);

	if (HealNumbersClass)
	{
		SpawnNumbers(HealNumbersClass, HealAmount);
	}

	if (HealthInterfaceOwner)
	{
		HealthInterfaceOwner->OnHealed(HealAmount);
	}
}

void UHealthComponent::SpawnNumbers(TSubclassOf<ADamageNumbers> NumbersClass, float Value)
{
	if (NumbersClass)
	{
		FVector NumbersLocation = Owner->GetActorLocation();

		float RandomAngle = FMath::RandRange(-180.0f, 180.0f);
		NumbersLocation.X += NumbersSpawnRadius * FMath::Cos(RandomAngle);
		NumbersLocation.Y += NumbersSpawnRadius * FMath::Sin(RandomAngle);

		ADamageNumbers* Numbers = World->SpawnActor<ADamageNumbers>(NumbersClass, NumbersLocation, Owner->GetActorRotation());
		if (Numbers)
		{
			Numbers->SpawnDamageNumbers(Value);
		}
	}
}

float UHealthComponent::GetHealthPercent()
{
	return CurrentHealth / MaxHealth;
}
