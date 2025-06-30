// INVI_1998 All Rights Reserved.


#include "FrontendSettings/FrontendGameUserSettings.h"

#include "FrontendSettings/FrontendDeveloperSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

UFrontendGameUserSettings::UFrontendGameUserSettings()
	: MasterVolume(1.f), 
	  MusicVolume(1.f), 
	  SFXVolume(1.f),
	  UserInterfaceVolume(1.f),
	  InGameMusicVolume(1.f),
	  MenuMusicVolume(1.f)
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

// 修改原有的音量设置函数
void UFrontendGameUserSettings::SetMasterVolume(float InMasterVolume)
{
    const UFrontendDeveloperSettings* FrontendSettings = GetDefault<UFrontendDeveloperSettings>();
    SetVolume(InMasterVolume, FrontendSettings->MasterSoundClassPath, MasterVolume);
}

void UFrontendGameUserSettings::SetMusicVolume(float InMusicVolume)
{
    const UFrontendDeveloperSettings* FrontendSettings = GetDefault<UFrontendDeveloperSettings>();
    SetVolume(InMusicVolume, FrontendSettings->MusicSoundClassPath, MusicVolume);
}

void UFrontendGameUserSettings::SetSFXVolume(float InSFXVolume)
{
    const UFrontendDeveloperSettings* FrontendSettings = GetDefault<UFrontendDeveloperSettings>();
    SetVolume(InSFXVolume, FrontendSettings->SFXSoundClassPath, SFXVolume);
}

void UFrontendGameUserSettings::SetUserInterfaceVolume(float InUserInterfaceVolume)
{
    const UFrontendDeveloperSettings* FrontendSettings = GetDefault<UFrontendDeveloperSettings>();
    SetVolume(InUserInterfaceVolume, FrontendSettings->UserInterfaceSoundClassPath, UserInterfaceVolume);
}

void UFrontendGameUserSettings::SetInGameMusicVolume(float InInGameMusicVolume)
{
    const UFrontendDeveloperSettings* FrontendSettings = GetDefault<UFrontendDeveloperSettings>();
    SetVolume(InInGameMusicVolume, FrontendSettings->InGameMusicSoundClassPath, InGameMusicVolume);
}

void UFrontendGameUserSettings::SetMenuMusicVolume(float InMenuMusicVolume)
{
    const UFrontendDeveloperSettings* FrontendSettings = GetDefault<UFrontendDeveloperSettings>();
    SetVolume(InMenuMusicVolume, FrontendSettings->MenuMusicSoundClassPath, MenuMusicVolume);
}

// 添加一个通用的私有函数
void UFrontendGameUserSettings::SetVolume(float InVolume, const FSoftObjectPath& SoundClassPath, float& VolumeVariable)
{
	UWorld* InAudioWorld = nullptr;
	const UFrontendDeveloperSettings* FrontendSettings = GetDefault<UFrontendDeveloperSettings>();

	if (GEngine)
	{
		InAudioWorld = GEngine->GetCurrentPlayWorld();
	}

	if (!InAudioWorld || !FrontendSettings)
	{
		return; // 如果没有音频世界或前端设置，则直接返回
	}

	USoundClass* SoundClass = nullptr;
	if (UObject* SoundClassObject = SoundClassPath.TryLoad())
	{
		SoundClass = Cast<USoundClass>(SoundClassObject);
	}

	USoundMix* SoundMix = nullptr;
	if (UObject* SoundMixObject = FrontendSettings->DefaultSoundMixPath.TryLoad())
	{
		SoundMix = Cast<USoundMix>(SoundMixObject);
	}

	VolumeVariable = InVolume;

	UGameplayStatics::SetSoundMixClassOverride(
		InAudioWorld,
		SoundMix,
		SoundClass,
		InVolume,
		1.f, // 音量衰减
		0.2f  // 混响衰减
	);

	UGameplayStatics::PushSoundMixModifier(InAudioWorld, SoundMix);
}

