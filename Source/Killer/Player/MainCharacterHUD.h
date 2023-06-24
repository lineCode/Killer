#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainCharacterHUD.generated.h"

class UHUDWidget;

UCLASS()
class KILLER_API AMainCharacterHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMainCharacterHUD();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category="HUD")
	UHUDWidget* HUDWidget;

public:
	UHUDWidget* GetHUDWidget() const { return HUDWidget; }
};
