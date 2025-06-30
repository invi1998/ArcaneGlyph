// INVI_1998 All Rights Reserved.


#include "FrontendSettings/FrontendGameUserSettings.h"

#include "FrontendSettings/FrontendDeveloperSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

UFrontendGameUserSettings::UFrontendGameUserSettings()
	: MasterVolume(1.f), 
	  MusicVolume(1.f), 
	  SFXVolume(1.f)
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

	USoundClass* MasterSoundClass = nullptr;
	if (UObject* MasterSoundClassObject = FrontendSettings->MasterSoundClassPath.TryLoad())
	{
		MasterSoundClass = Cast<USoundClass>(MasterSoundClassObject);
	}

	USoundMix* MasterSoundMix = nullptr;
	if (UObject* MasterSoundMixObject = FrontendSettings->DefaultSoundMixPath.TryLoad())
	{
		MasterSoundMix = Cast<USoundMix>(MasterSoundMixObject);
	}

	MasterVolume = InMasterVolume;

	UGameplayStatics::SetSoundMixClassOverride(
		InAudioWorld,
		MasterSoundMix,
		MasterSoundClass,
		InMasterVolume,
		1.f, // 音量衰减
		0.2f  // 混响衰减
	);

	UGameplayStatics::PushSoundMixModifier(InAudioWorld, MasterSoundMix);

}

void UFrontendGameUserSettings::SetMusicVolume(float InMusicVolume)
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
	USoundClass* MusicSoundClass = nullptr;
	if (UObject* MusicSoundClassObject = FrontendSettings->MusicSoundClassPath.TryLoad())
	{
		MusicSoundClass = Cast<USoundClass>(MusicSoundClassObject);
	}

	USoundMix* MasterSoundMix = nullptr;
	if (UObject* MasterSoundMixObject = FrontendSettings->DefaultSoundMixPath.TryLoad())
	{
		MasterSoundMix = Cast<USoundMix>(MasterSoundMixObject);
	}

	MusicVolume = InMusicVolume;
	
	UGameplayStatics::SetSoundMixClassOverride(
		InAudioWorld,
		MasterSoundMix,
		MusicSoundClass,
		InMusicVolume,
		1.f, // 音量衰减
		0.2f  // 混响衰减
	);
	UGameplayStatics::PushSoundMixModifier(InAudioWorld, MasterSoundMix);
}

void UFrontendGameUserSettings::SetSFXVolume(float InSFXVolume)
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
	USoundClass* SFXSoundClass = nullptr;
	if (UObject* SFXSoundClassObject = FrontendSettings->SFXSoundClassPath.TryLoad())
	{
		SFXSoundClass = Cast<USoundClass>(SFXSoundClassObject);
	}

	USoundMix* SFXSoundMix = nullptr;
	if (UObject* SFXSoundMixObject = FrontendSettings->DefaultSoundMixPath.TryLoad())
	{
		SFXSoundMix = Cast<USoundMix>(SFXSoundMixObject);
	}

	SFXVolume = InSFXVolume;
	UGameplayStatics::SetSoundMixClassOverride(
		InAudioWorld,
		SFXSoundMix,
		SFXSoundClass,
		InSFXVolume,
		1.f, // 音量衰减
		0.2f  // 混响衰减
	);
	UGameplayStatics::PushSoundMixModifier(InAudioWorld, SFXSoundMix);
	
}
