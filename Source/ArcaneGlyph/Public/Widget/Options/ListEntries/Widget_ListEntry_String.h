// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget_ListEntry_Base.h"
#include "Widget_ListEntry_String.generated.h"

class UListDataObject_String;
class UFrontendCommonButtonBase;
class UFrontendCommonRotator;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_ListEntry_String : public UWidget_ListEntry_Base
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void OnOwningListDataObjectModified(UListDataObject_Base* InListDataObject, EOptionsListDataModifyReason InOptionsListDataModifyReason) override;
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	
	void OnPreviousOptionClicked();
	void OnNextOptionClicked();
	void OnRotatorOptionClicked();
	void OnRotatorOptionChanged(int32 InSelectedIndex, bool bUserInitiated);

private:
	// **** Bound Widgets **** //
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_PreviousOption;	// 上一个选项按钮

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonRotator* CommonRotator_AvailableOptions;	// 可用选项旋转器（选项选择器）

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_NextOption;	// 下一个选项按钮
	// **** Bound Widgets **** //

	UPROPERTY(Transient)
	UListDataObject_String* OwningStringDataObject;	// 拥有的字符串数据对象，用于处理选项的选中状态和显示文本等逻辑
};
