// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FrontendLoadingScreenSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class ARCANEGLYPH_API UFrontendLoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	TSubclassOf<UUserWidget> GetLoadingScreenWidgetClassChecked() const;
	
	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	TSoftClassPtr<UUserWidget> SoftLoadingScreenWidgetClass;	// 异步加载的加载屏幕小部件类

	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	float HoldLoadingScreenExtraSeconds = 5.0f;	// 在加载完成后额外保持加载屏幕的时间（秒）加载屏幕设置中的这个变量用于消除模糊纹理

	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	bool bShouldLoadingScreenInEditor = false;	// 是否在编辑器中显示加载屏幕

	
	
};
