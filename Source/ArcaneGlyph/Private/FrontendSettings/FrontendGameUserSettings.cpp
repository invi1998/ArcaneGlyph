// INVI_1998 All Rights Reserved.


#include "FrontendSettings/FrontendGameUserSettings.h"

#include "FrontendSettings/FrontendDeveloperSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

UFrontendGameUserSettings::UFrontendGameUserSettings()
		: bTutorialModeEnabled(true),
		bAutoTargetLock(true),
		bAutoAttackTargetLock(true),
		MasterVolume(1.f), 
		MusicVolume(1.f), 
		SFXVolume(1.f),
		UserInterfaceVolume(1.f),
		InGameMusicVolume(1.f),
		MenuMusicVolume(1.f),
		bAllowBackgroundAudio(true),
		bUseHDRAudio(false)
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

void UFrontendGameUserSettings::SetCurrentGameplayTutorialModeEnabled(bool InTutorialModeEnabled)
{
	bTutorialModeEnabled = InTutorialModeEnabled;
}

void UFrontendGameUserSettings::SetCurrentGameplayAutoTargetLock(bool InAutoTargetLock)
{
	bAutoTargetLock = InAutoTargetLock;
}

void UFrontendGameUserSettings::SetCurrentGameplayAutoAttackTargetLock(bool InAutoAttackTargetLock)
{
	bAutoAttackTargetLock = InAutoAttackTargetLock;
}

// 修改原有的音量设置函数
void UFrontendGameUserSettings::SetMasterVolume(float InMasterVolume)
{
    const UFrontendDeveloperSettings* FrontendSettings = GetDefault<UFrontendDeveloperSettings>();
    SetVolume(InMasterVolume, FrontendSettings->MasterSoundClassPath, FrontendSettings->MasterSoundMixPath, MasterVolume);
}

void UFrontendGameUserSettings::SetMusicVolume(float InMusicVolume)
{
    const UFrontendDeveloperSettings* FrontendSettings = GetDefault<UFrontendDeveloperSettings>();
    SetVolume(InMusicVolume, FrontendSettings->MusicSoundClassPath, FrontendSettings->MusicSoundMixPath, MusicVolume);
}

void UFrontendGameUserSettings::SetSFXVolume(float InSFXVolume)
{
    const UFrontendDeveloperSettings* FrontendSettings = GetDefault<UFrontendDeveloperSettings>();
    SetVolume(InSFXVolume, FrontendSettings->SFXSoundClassPath, FrontendSettings->VFXSoundMixPath, SFXVolume);
}

void UFrontendGameUserSettings::SetUserInterfaceVolume(float InUserInterfaceVolume)
{
    const UFrontendDeveloperSettings* FrontendSettings = GetDefault<UFrontendDeveloperSettings>();
    SetVolume(InUserInterfaceVolume, FrontendSettings->UserInterfaceSoundClassPath, FrontendSettings->UISoundMixPath, UserInterfaceVolume);
}

void UFrontendGameUserSettings::SetInGameMusicVolume(float InInGameMusicVolume)
{
    const UFrontendDeveloperSettings* FrontendSettings = GetDefault<UFrontendDeveloperSettings>();
    SetVolume(InInGameMusicVolume, FrontendSettings->InGameMusicSoundClassPath, FrontendSettings->InGameMusicSoundMixPath, InGameMusicVolume);
}

void UFrontendGameUserSettings::SetMenuMusicVolume(float InMenuMusicVolume)
{
    const UFrontendDeveloperSettings* FrontendSettings = GetDefault<UFrontendDeveloperSettings>();
    SetVolume(InMenuMusicVolume, FrontendSettings->MenuMusicSoundClassPath, FrontendSettings->MenuMusicSoundMixPath, MenuMusicVolume);
}

void UFrontendGameUserSettings::SetAllowBackgroundAudio(bool bInAllowBackgroundAudio)
{
	bAllowBackgroundAudio = bInAllowBackgroundAudio;
}

void UFrontendGameUserSettings::SetUseHDRAudio(bool bInUseHDRAudio)
{
	bUseHDRAudio = bInUseHDRAudio;
}

float UFrontendGameUserSettings::GetDisplayGama() const
{
	if (GEngine)
	{
		return GEngine->GetDisplayGamma();
	}

	return 0.f;
}

void UFrontendGameUserSettings::SetDisplayGama(float InDisplayGama)
{
	if (GEngine)
	{
		GEngine->DisplayGamma = InDisplayGama;
	}
}

// 添加一个通用的私有函数
void UFrontendGameUserSettings::SetVolume(float InVolume, const FSoftObjectPath& SoundClassPath, const FSoftObjectPath& SoundMixPath, float& VolumeVariable)
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
	if (UObject* SoundMixObject = SoundMixPath.TryLoad())
	{
		SoundMix = Cast<USoundMix>(SoundMixObject);
	}

	if (!SoundClass || !SoundMix)
	{
		return; // 确保 SoundClass 和 SoundMix 都已加载
	}

	// 1. 更新成员变量
	VolumeVariable = InVolume;

	// 2. 设置 Sound Mix 覆盖
	UGameplayStatics::SetSoundMixClassOverride(
		InAudioWorld,
		SoundMix,
		SoundClass,
		InVolume,
		1.f, // 音量衰减
		0.2f  // 混响衰减
	);

	// 3. 移除重复推送 Sound Mix 的代码
	// 每次设置音量时，不应该重复推送 Sound Mix。
	// 只需要确保 Sound Mix 在游戏开始时被推送一次即可。
	// UGameplayStatics::PushSoundMixModifier(InAudioWorld, SoundMix); // <-- 移除或注释掉这
}

