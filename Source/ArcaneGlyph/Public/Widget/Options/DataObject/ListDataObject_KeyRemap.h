// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "ListDataObject_Base.h"
#include "ListDataObject_KeyRemap.generated.h"

class UEnhancedPlayerMappableKeyProfile;
class UEnhancedInputUserSettings;
/**
 * 键位重映射列表数据对象类
 */
UCLASS()
class ARCANEGLYPH_API UListDataObject_KeyRemap : public UListDataObject_Base
{
	GENERATED_BODY()

public:
	void InitKeyRemapData(
		ECommonInputType InDesiredInputType,
		UEnhancedInputUserSettings* InEnhancedInputUserSettings,
		UEnhancedPlayerMappableKeyProfile* InPlayerMappableKeyProfile,
		const FPlayerKeyMapping& InPlayerKeyMapping,
		const FPlayerKeyMapping* InTriggerKeyMapping
	);

	FORCEINLINE ECommonInputType GetDesiredInputType() const { return CachedDesiredInputType; }	// 获取当前期望的输入类型（键盘鼠标或手柄）

	FSlateBrush GetIconFromCurrentKey() const;
	FSlateBrush GetTriggerIconFromCurrentKey() const;

	bool HasTrigger = false;	// 是否有触发键位映射

	void BindNewInputKey(const FKey& InNewKey);		// 绑定新的输入键位映射

	// Begin UListDataObject_Base Interface
	virtual bool HasDefaultValue() const override;
	virtual bool CanResetToDefault() const override;
	virtual bool TryResetToDefault() override;
	// End UListDataObject_Base Interface

protected:


private:
	FPlayerKeyMapping* GetOwningKeyMapping(const FName& InMappingName, const EPlayerMappableKeySlot& InSlot) const;	// 获取当前拥有的键位映射
	
	UPROPERTY(Transient)
	UEnhancedInputUserSettings* CachedEnhancedInputUserSettings;	// 引用Enhanced Input User Settings，用于获取和设置键位映射

	UPROPERTY(Transient)
	UEnhancedPlayerMappableKeyProfile* CachedPlayerMappableKeyProfile;	// 引用玩家可映射键位配置文件，用于获取和设置键位映射
	
	ECommonInputType CachedDesiredInputType;	// 缓存的输入类型，用于判断当前使用的是哪种输入设备（键盘鼠标或手柄）
	
	FName CachedMappingName;	// 缓存的映射名称，用于标识当前键位映射

	EPlayerMappableKeySlot CachedMappableKeySlot;	// 缓存的键位槽，用于标识当前键位映射的槽位（如主攻击、次攻击等）

	// 触发键位映射，用于在设置键位时进行预处理
	FName CachedTriggerName;	// 触发键位映射的名称

	// 触发键位映射的槽位
	EPlayerMappableKeySlot CachedTriggerKeySlot;	// 触发键位映射的槽位，用于标识当前触发键位映射的槽位（如主攻击、次攻击等）

};
