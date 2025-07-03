// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "FrontendDeveloperSettings.generated.h"

class UWidget_ActivatableBase;
/**
 * 我希望能像配置游戏默认地图那样配置我们游戏里需要用到的Widget
 * 这样就可以在游戏中通过标签来获取对应的Widget类了
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Frontend UI Settings"))
class ARCANEGLYPH_API UFrontendDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Widget References", meta=(Categories = "Frontend.Widget", ForceInlineRow))
	TMap<FGameplayTag, TSoftClassPtr<UWidget_ActivatableBase>> FrontendWidgetMap;	// 前端小部件映射

	UPROPERTY(Config, EditAnywhere, Category="Options Image References", meta=(Categories = "Frontend.Image", ForceInlineRow))
	TMap<FGameplayTag, TSoftObjectPtr<UTexture2D>> FrontendOptionsSoftImageMap;	// 前端选项图片映射

	UPROPERTY(Config, EditAnywhere, Category="Options Description ST", meta = (AllowedClasses = "/Script/Engine.StringTable", ForceInlineRow))
	FSoftObjectPath OptionsDescriptionSTPath;	// 选项描述文本资源路径，用于设置选项描述文本
	
	UPROPERTY(Config, EditAnywhere, Category="Audio Settings", meta=(AllowedClasses = "/Script/Engine.SoundClass", ForceInlineRow))
	FSoftObjectPath MasterSoundClassPath;	// 主音效类路径，用于设置主音量

	UPROPERTY(Config, EditAnywhere, Category="Audio Settings", meta=(AllowedClasses = "/Script/Engine.SoundClass", ForceInlineRow))
	FSoftObjectPath MusicSoundClassPath;	// 音乐音效类路径，用于设置音乐音量

	UPROPERTY(Config, EditAnywhere, Category="Audio Settings", meta=(AllowedClasses = "/Script/Engine.SoundClass", ForceInlineRow))
	FSoftObjectPath SFXSoundClassPath;	// 音效音效类路径，用于设置音效音量

	UPROPERTY(Config, EditAnywhere, Category="Audio Settings", meta=(AllowedClasses = "/Script/Engine.SoundClass", ForceInlineRow))
	FSoftObjectPath UserInterfaceSoundClassPath;	// 用户界面音效类路径，用于设置用户界面音量
	
	UPROPERTY(Config, EditAnywhere, Category="Audio Settings", meta=(AllowedClasses = "/Script/Engine.SoundClass", ForceInlineRow))
	FSoftObjectPath InGameMusicSoundClassPath;	// 游戏内音乐音效类路径，用于设置游戏内音乐音量

	UPROPERTY(Config, EditAnywhere, Category="Audio Settings", meta=(AllowedClasses = "/Script/Engine.SoundClass", ForceInlineRow))
	FSoftObjectPath MenuMusicSoundClassPath;	// 菜单音乐音效类路径，用于设置菜单音乐音量

	UPROPERTY(Config, EditAnywhere, Category="Audio Settings", meta=(AllowedClasses = "/Script/Engine.SoundMix", ForceInlineRow))
	FSoftObjectPath DefaultSoundMixPath;	// 默认音效混合路径，用于设置默认音效混合



};
