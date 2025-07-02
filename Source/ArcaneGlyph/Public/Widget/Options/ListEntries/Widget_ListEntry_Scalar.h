// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget_ListEntry_Base.h"
#include "Widget_ListEntry_Scalar.generated.h"

class UListDataObject_Scalar;
class UCommonNumericTextBlock;
class UAnalogSlider;

/**
 * 滑条列表项小部件类
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_ListEntry_Scalar : public UWidget_ListEntry_Base
{
	GENERATED_BODY()

protected:
	// Begin UUserWidget interface
	virtual void NativeOnInitialized() override;
	// End UUserWidget interface

	// Begin UWidget_ListEntry_Base Interface
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* InListDataObject, EOptionsListDataModifyReason InOptionsListDataModifyReason) override;
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	virtual void OnToggleEditableState(bool bIsEditable) override;
	// End UWidget_ListEntry_Base Interface

	UFUNCTION()
	void OnSettingSliderValueChanged(float Value);

	UFUNCTION()
	void OnSettingSliderMouseCaptureBegin();

	UFUNCTION()
	void OnSettingSliderMouseCaptureEnd();

private:
	// **** Bound Widgets **** //
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UCommonNumericTextBlock* CommonNumeric_SettingValue;	// 数值文本块，用于显示滑条当前数值

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UAnalogSlider* AnalogSlider_SettingSlider;	// 滑条控件，用于调整数值（这个控件是CommonUI的滑条控件，要实现手柄控制就得用这个滑条）
	// **** Bound Widgets **** //

	UPROPERTY(Transient)
	UListDataObject_Scalar* OwningScalarDataObject;	// 拥有的标量数据对象，用于处理选项的选中状态和显示文本等逻辑

};
