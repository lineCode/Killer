#include "FunctionLibrary.h"
#include "Killer/Combat/ParticlesAndSound.h"

USave* UFunctionLibrary::GetSave()
{
	USaveGame* EmptySave = UGameplayStatics::CreateSaveGameObject(USave::StaticClass());
	if (!EmptySave) return nullptr;

	USave* Save = Cast<USave>(EmptySave);
	if (!Save) return nullptr;

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
