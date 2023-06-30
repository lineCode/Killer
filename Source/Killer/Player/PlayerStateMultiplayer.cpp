#include "PlayerStateMultiplayer.h"
#include "MainCharacterController.h"
#include "MainCharacterHUDMultiplayer.h"
#include "Killer/UI/HUD/PlayersTableWidget.h"
#include "Net/UnrealNetwork.h"

APlayerStateMultiplayer::APlayerStateMultiplayer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APlayerStateMultiplayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerStateMultiplayer, KillsCount);
	DOREPLIFETIME(APlayerStateMultiplayer, DeathsCount);
}

void APlayerStateMultiplayer::Server_IncrementKillsCount_Implementation()
{
	KillsCount++;
	
	OnRep_KillsCount();
}

void APlayerStateMultiplayer::Server_IncrementDeathsCount_Implementation()
{
	DeathsCount++;

	OnRep_DeathsCount();
}

void APlayerStateMultiplayer::OnRep_KillsCount()
{
	RefreshPlayersTable();
}

void APlayerStateMultiplayer::OnRep_DeathsCount()
{
	RefreshPlayersTable();
}

void APlayerStateMultiplayer::RefreshPlayersTable() const
{
	if (const auto* MainCharacterController = Cast<AMainCharacterController>(GetOwningController()))
	{
		if (const auto* MainCharacterHUD = Cast<AMainCharacterHUDMultiplayer>(MainCharacterController->GetHUD()))
		{
			MainCharacterHUD->GetPlayersTableWidget()->RefreshPlayersTable();
		}
	}
}
