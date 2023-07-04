#include "Save.h"
#include "Kismet/GameplayStatics.h"

USave::USave()
{
	SlotName = "SaveSlot";

	if (const auto GunClass = ConstructorHelpers::FObjectFinder<UClass>(
		TEXT("Class'/Game/Blueprints/Combat/Weapons/BP_Gun.BP_Gun_C'")); GunClass.Succeeded())
	{
		WeaponClass = GunClass.Object;
	}

	LastChosenLevelName = "MidnightCity";

	EffectsVolume = 0.5f;
	MusicVolume = 0.5f;
	UIVolume = 0.5f;
	FOVShake = 0.5f;

	PlayerName = "Player";
	
	if (const auto Material = ConstructorHelpers::FObjectFinder<UMaterialInterface>(
		TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Graphics/Materials/SpriteLit/MI_SL_Red.MI_SL_Red'")); Material.Succeeded())
	{
		PlayerMaterial = Material.Object;
	}
}

USave* USave::GetSave()
{
	USaveGame* EmptySave = UGameplayStatics::CreateSaveGameObject(StaticClass());
	if (!EmptySave)
	{
		return nullptr;
	}

	auto* Save = Cast<USave>(EmptySave);
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
