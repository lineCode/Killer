﻿#pragma once

#include "CoreMinimal.h"
#include "Killer/Player/General/MainCharacterHUD.h"
#include "MainCharacterHUDMultiplayer.generated.h"

class UPlayersTableWidget;

UCLASS()
class KILLER_API AMainCharacterHUDMultiplayer : public AMainCharacterHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Main Character HUD")
	TSubclassOf<UUserWidget> PlayersTableWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UPlayersTableWidget* PlayersTableWidget;

public:
	AMainCharacterHUDMultiplayer();

	UPlayersTableWidget* GetPlayersTableWidget() const { return PlayersTableWidget; }

	void ShowPlayersTableWidget() const;
	void HidePlayersTableWidget() const;
};
