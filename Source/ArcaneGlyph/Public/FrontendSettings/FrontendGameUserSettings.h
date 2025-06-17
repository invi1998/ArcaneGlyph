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
	static UFrontendGameUserSettings* Get();

	//***** GamePlay Settings *****//
	// 获取教程模式启用状态
	UFUNCTION()
	FString GetCurrentGameplayTutorialModeEnabled() const { return TutorialModeEnabled; }

	UFUNCTION()
	void SetCurrentGameplayTutorialModeEnabled(const FString& InTutorialModeEnabled);

	

private:
	UPROPERTY(Config)
	FString TutorialModeEnabled;	// 教程模式启用状态，存储在配置文件中
};
