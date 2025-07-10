// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget_ListEntry_Base.h"
#include "Widget_ListEntry_KeyRemap.generated.h"

class UFrontendCommonButtonBase;
class UListDataObject_KeyRemap;
/**
 * 键位重映射列表项小部件类
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_ListEntry_KeyRemap : public UWidget_ListEntry_Base
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	// Begin UWidget_ListEntry_Base Interface
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* InListDataObject, EOptionsListDataModifyReason InOptionsListDataModifyReason) override;
	// End UWidget_ListEntry_Base Interface
	
private:
	void OnRemapKeyButtonClicked();
	void OnResetKeyBindingButtonClicked();
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_TriggerKey;	// 触发键位重映射按钮，组合键位里的前置触发键位（该项一帮不会被编辑）

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonTextBlock_ConnectedKey;	// 连接文本块 (+) 用于显示组合关系
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_KeyRemap;	// 键位重映射按钮，用于触发键位重映射操作

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_ResetKeyBinding;	// 重置键位绑定按钮，用于重置当前键位绑定到默认值

	UPROPERTY()
	UListDataObject_KeyRemap* CachedOwningKeyRemapDataObject;	// 缓存的拥有的键位重映射数据对象，用于处理选项的选中状态和显示文本等逻辑
};
