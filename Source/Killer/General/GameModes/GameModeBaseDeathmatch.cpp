#include "GameModeBaseDeathmatch.h"
#include "Killer/Player/Multiplayer/MainCharacterControllerMultiplayer.h"
#include "Killer/Player/Multiplayer/MainCharacterStateMultiplayer.h"
#include "Kismet/GameplayStatics.h"

AGameModeBaseDeathmatch::AGameModeBaseDeathmatch()
{
	PrimaryActorTick.bCanEverTick = false;

	TimeToEndMatch = 300;
	TimeToRestartMatch = 10.0f;
}

void AGameModeBaseDeathmatch::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!GetWorldTimerManager().IsTimerActive(EndMatchTimerHandle))
	{
		GetWorldTimerManager().SetTimer(EndMatchTimerHandle, this, &AGameModeBaseDeathmatch::EndMatch,
		TimeToEndMatch, false, TimeToEndMatch);
	}

	if (auto* MultiplayerController = Cast<AMainCharacterControllerMultiplayer>(NewPlayer))
	{
		MultiplayerController->Client_ShowTimer(GetWorldTimerManager().GetTimerRemaining(EndMatchTimerHandle));
	}
}

void AGameModeBaseDeathmatch::EndMatch()
{
	int32 MaxKills = 0;
	for (const auto* Controller : PlayerControllers)
	{
		if (const int32 KillsCount = Controller->GetPlayerState<AMainCharacterStateMultiplayer>()->GetKillsCount(); KillsCount > MaxKills)
		{
			MaxKills = KillsCount;
		}
	}

	for (auto* Controller : PlayerControllers)
	{
		if (Controller->GetPlayerState<AMainCharacterStateMultiplayer>()->GetKillsCount() >= MaxKills)
		{
			Controller->Client_ShowTextMessage(WinMessage);
		}
		else
		{
			Controller->Client_ShowTextMessage(LossMessage);
		}

		Controller->Client_ShowTimer(TimeToRestartMatch);
	}

	GetWorldTimerManager().SetTimer(RestartMatchTimerHandle, this, &AGameModeBaseDeathmatch::RestartMatch,
		TimeToRestartMatch, false, TimeToRestartMatch);
}

void AGameModeBaseDeathmatch::RestartMatch()
{
	if (DeathmatchMapsURLs.Num() > 0)
	{
		GetWorld()->ServerTravel(DeathmatchMapsURLs[FMath::RandRange(0, DeathmatchMapsURLs.Num() - 1)]);
	}
	else
	{
		GetWorld()->ServerTravel(UGameplayStatics::GetCurrentLevelName(this));
	}
}