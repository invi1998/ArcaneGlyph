// INVI_1998 All Rights Reserved.


#include "FrontendSettings/FrontendGameUserSettings.h"

UFrontendGameUserSettings::UFrontendGameUserSettings()
	: MasterVolume(1.f)
{
}

UFrontendGameUserSettings* UFrontendGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UFrontendGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}

void UFrontendGameUserSettings::SetCurrentGameplayTutorialModeEnabled(const FString& InTutorialModeEnabled)
{
	TutorialModeEnabled = InTutorialModeEnabled;
}

void UFrontendGameUserSettings::SetMasterVolume(float InMasterVolume)
{
	MasterVolume = InMasterVolume;

}
