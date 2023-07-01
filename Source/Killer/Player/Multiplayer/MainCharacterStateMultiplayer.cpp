#include "MainCharacterStateMultiplayer.h"
#include "Net/UnrealNetwork.h"

AMainCharacterStateMultiplayer::AMainCharacterStateMultiplayer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMainCharacterStateMultiplayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainCharacterStateMultiplayer, KillsCount);
	DOREPLIFETIME(AMainCharacterStateMultiplayer, DeathsCount);
}

void AMainCharacterStateMultiplayer::Server_IncrementKillsCount_Implementation()
{
	KillsCount++;
}

void AMainCharacterStateMultiplayer::Server_IncrementDeathsCount_Implementation()
{
	DeathsCount++;
}
