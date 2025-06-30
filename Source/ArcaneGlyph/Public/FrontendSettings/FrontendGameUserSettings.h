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
	FString GetCurrentGameplayTutorialModeEnabled() const { return TutorialModeEnabled; }

	UFUNCTION()
	void SetCurrentGameplayTutorialModeEnabled(const FString& InTutorialModeEnabled);
	
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
	

private:
	void SetVolume(float InVolume, const FSoftObjectPath& SoundClassPath, float& VolumeVariable);

	//***** GamePlay Settings *****//
	UPROPERTY(Config)
	FString TutorialModeEnabled;	// 教程模式启用状态，存储在配置文件中

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
};
