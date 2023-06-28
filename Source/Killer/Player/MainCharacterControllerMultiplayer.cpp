#include "MainCharacterControllerMultiplayer.h"
#include "MainCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AMainCharacterControllerMultiplayer::Restart()
{
	Server_Restart();
}

void AMainCharacterControllerMultiplayer::Server_Restart_Implementation()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (!MainCharacter || (MainCharacter && !MainCharacter->GetHealthComponent()->IsDead()))
	{
		return;
	}
	
	TArray<AActor*> AllPlayerSpawns;
	UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), AllPlayerSpawns);

	if (AllPlayerSpawns.Num() > 0)
	{
		MainCharacter->SetActorLocation(AllPlayerSpawns[FMath::RandRange(0, AllPlayerSpawns.Num() - 1)]->GetActorLocation());
	}

	MainCharacter->GetHealthComponent()->Server_ReviveOwner();
}