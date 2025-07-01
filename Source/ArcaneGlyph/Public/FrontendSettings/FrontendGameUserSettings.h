// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FrontendGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UFrontendGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UFrontendGameUserSettings();
	
	static UFrontendGameUserSettings* Get();

	//***** GamePlay Settings *****//
	// 获取教程模式启用状态
	UFUNCTION()
	bool GetCurrentGameplayTutorialModeEnabled() const { return bTutorialModeEnabled; }

	UFUNCTION()
	void SetCurrentGameplayTutorialModeEnabled(bool InTutorialModeEnabled);

	UFUNCTION()
	bool GetCurrentGameplayAutoTargetLock() const { return bAutoTargetLock; }

	UFUNCTION()
	void SetCurrentGameplayAutoTargetLock(bool InAutoTargetLock);

	UFUNCTION()
	bool GetCurrentGameplayAutoAttackTargetLock() const { return bAutoAttackTargetLock; }

	UFUNCTION()
	void SetCurrentGameplayAutoAttackTargetLock(bool InAutoAttackTargetLock);
	
	//***** Audio Collection Settings *****//
	// 获取主音量
	UFUNCTION()
	float GetMasterVolume() const { return MasterVolume; }

	UFUNCTION()
	void SetMasterVolume(float InMasterVolume);

	UFUNCTION()
	float GetMusicVolume() const { return MusicVolume; }

	UFUNCTION()
	void SetMusicVolume(float InMusicVolume);

	UFUNCTION()
	float GetSFXVolume() const { return SFXVolume; }

	UFUNCTION()
	void SetSFXVolume(float InSFXVolume);

	UFUNCTION()
	float GetUserInterfaceVolume() const { return UserInterfaceVolume; }

	UFUNCTION()
	void SetUserInterfaceVolume(float InUserInterfaceVolume);

	UFUNCTION()
	float GetInGameMusicVolume() const { return InGameMusicVolume; }

	UFUNCTION()
	void SetInGameMusicVolume(float InInGameMusicVolume);

	UFUNCTION()
	float GetMenuMusicVolume() const { return MenuMusicVolume; }

	UFUNCTION()
	void SetMenuMusicVolume(float InMenuMusicVolume);

	UFUNCTION()
	bool GetAllowBackgroundAudio() const { return bAllowBackgroundAudio; }

	UFUNCTION()
	void SetAllowBackgroundAudio(bool bInAllowBackgroundAudio);

	UFUNCTION()
	bool GetUseHDRAudio() const { return bUseHDRAudio; }

	UFUNCTION()
	void SetUseHDRAudio(bool bInUseHDRAudio);

	// ***** Audio Collection Settings *****//

private:
	void SetVolume(float InVolume, const FSoftObjectPath& SoundClassPath, float& VolumeVariable);

	//***** GamePlay Settings *****//
	UPROPERTY(Config)
	bool bTutorialModeEnabled;	// 教程模式启用状态，存储在配置文件中

	UPROPERTY(Config)
	bool bAutoTargetLock;	// 自动锁定目标

	UPROPERTY(Config)
	bool bAutoAttackTargetLock;		// 自动攻击目标锁定
	

	//***** Audio Collection Settings *****//
	UPROPERTY(Config)
	float MasterVolume;		// 主音量

	UPROPERTY(Config)
	float MusicVolume;		// 音乐音量

	UPROPERTY(Config)
	float SFXVolume;		// 音效音量

	UPROPERTY(Config)
	float UserInterfaceVolume;	// 用户界面音量

	UPROPERTY(Config)
	float InGameMusicVolume;	// 游戏内音乐音量

	UPROPERTY(Config)
	float MenuMusicVolume;		// 菜单音乐音量

	// Sound
	UPROPERTY(Config)
	bool bAllowBackgroundAudio;	// 是否允许后台音频播放

	UPROPERTY(Config)
	bool bUseHDRAudio;	// 是否使用高动态范围音频(HDR)

	//***** Video Settings *****//
};
