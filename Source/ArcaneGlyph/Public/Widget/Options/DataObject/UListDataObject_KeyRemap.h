// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "ListDataObject_Base.h"
#include "UListDataObject_KeyRemap.generated.h"

class UEnhancedPlayerMappableKeyProfile;
class UEnhancedInputUserSettings;
/**
 * 键位重映射列表数据对象类
 */
UCLASS()
class ARCANEGLYPH_API UUListDataObject_KeyRemap : public UListDataObject_Base
{
	GENERATED_BODY()

public:
	void InitKeyRemapData(
		ECommonInputType InDesiredInputType,
		UEnhancedInputUserSettings* InEnhancedInputUserSettings,
		UEnhancedPlayerMappableKeyProfile* InPlayerMappableKeyProfile,
		const FPlayerKeyMapping& InPlayerKeyMapping
	);

private:
	UPROPERTY(Transient)
	UEnhancedInputUserSettings* CachedEnhancedInputUserSettings;	// 引用Enhanced Input User Settings，用于获取和设置键位映射

	UPROPERTY(Transient)
	UEnhancedPlayerMappableKeyProfile* CachedPlayerMappableKeyProfile;	// 引用玩家可映射键位配置文件，用于获取和设置键位映射
	
	ECommonInputType CachedDesiredInputType;	// 缓存的输入类型，用于判断当前使用的是哪种输入设备（键盘鼠标或手柄）
	
	FName CachedMappingName;	// 缓存的映射名称，用于标识当前键位映射

	EPlayerMappableKeySlot CachedMappableKeySlot;	// 缓存的键位槽，用于标识当前键位映射的槽位（如主攻击、次攻击等）


};
