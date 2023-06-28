#include "PlayerStateMultiplayer.h"
#include "MainCharacterController.h"
#include "MainCharacterHUD.h"
#include "Killer/UI/HUDWidget.h"
#include "Killer/UI/PlayersPanelWidget.h"
#include "Net/UnrealNetwork.h"

APlayerStateMultiplayer::APlayerStateMultiplayer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APlayerStateMultiplayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerStateMultiplayer, PlayerDisplayName);
	DOREPLIFETIME(APlayerStateMultiplayer, KillsCount);
}

void APlayerStateMultiplayer::Server_IncrementKillsCount_Implementation()
{
	KillsCount++;
	
	OnRep_KillsCount();
}

void APlayerStateMultiplayer::OnRep_KillsCount()
{
	if (const auto* MainCharacterController = Cast<AMainCharacterController>(GetOwningController()))
	{
		if (const auto* MainCharacterHUD = Cast<AMainCharacterHUD>(MainCharacterController->GetHUD()))
		{
			MainCharacterHUD->GetHUDWidget()->GetPlayersPanelWidget()->DisplayKillsCount(KillsCount);
		}
	}
}