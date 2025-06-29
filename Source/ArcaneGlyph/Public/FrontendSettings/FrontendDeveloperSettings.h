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

};
