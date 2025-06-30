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
	

private:
	//***** GamePlay Settings *****//
	UPROPERTY(Config)
	FString TutorialModeEnabled;	// 教程模式启用状态，存储在配置文件中

	//***** Audio Collection Settings *****//
	UPROPERTY(Config)
	float MasterVolume;		// 主音量

	UPROPERTY(Config)
	float MusicVolume;		// 音乐音量
};
