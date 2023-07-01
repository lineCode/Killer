#include "Save.h"
#include "Kismet/GameplayStatics.h"

USave::USave()
{
	SlotName = "SaveSlot";

	if (const auto GunClass = ConstructorHelpers::FObjectFinder<UBlueprint>(
		TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Combat/Weapons/BP_Gun.BP_Gun'")); GunClass.Succeeded())
	{
		WeaponClass = GunClass.Object->GeneratedClass;
	}

	LastChosenLevelName = "MidnightCity";

	EffectsVolume = 0.5f;
	MusicVolume = 0.5f;
	UIVolume = 0.5f;
	FOVShake = 0.5f;
}

USave* USave::GetSave()
{
	USaveGame* EmptySave = UGameplayStatics::CreateSaveGameObject(StaticClass());
	if (!EmptySave)
	{
		return nullptr;
	}

	USave* Save = Cast<USave>(EmptySave);
	if (!Save)
	{
		return nullptr;
	}

	if (UGameplayStatics::DoesSaveGameExist(Save->SlotName, 0))
	{
		EmptySave = UGameplayStatics::LoadGameFromSlot(Save->SlotName, 0);
		if (EmptySave)
		{
			Save = Cast<USave>(EmptySave);
		}
	}

	return Save;
}
