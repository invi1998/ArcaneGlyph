// INVI_1998 All Rights Reserved.


#include "Widget/Options/ListEntries/Widget_ListEntry_Scalar.h"

#include "Widget/Options/DataObject/ListDataObject_Scalar.h"
#include "AnalogSlider.h"

void UWidget_ListEntry_Scalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AnalogSlider_SettingSlider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::OnSettingSliderValueChanged);
	AnalogSlider_SettingSlider->OnMouseCaptureBegin.AddUniqueDynamic(this, &ThisClass::OnSettingSliderMouseCaptureBegin);
	AnalogSlider_SettingSlider->OnMouseCaptureEnd.AddUniqueDynamic(this, &ThisClass::OnSettingSliderMouseCaptureEnd);
}

void UWidget_ListEntry_Scalar::OnOwningListDataObjectModified(UListDataObject_Base* InListDataObject, EOptionsListDataModifyReason InOptionsListDataModifyReason)
{
	if (OwningScalarDataObject)
	{
		// 更新滑块的当前值
		AnalogSlider_SettingSlider->SetValue(OwningScalarDataObject->GetCurrentScalarValue());

		// 更新数值显示文本
		CommonNumeric_SettingValue->SetCurrentValue(OwningScalarDataObject->GetCurrentScalarValue());
	}
}

void UWidget_ListEntry_Scalar::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	OwningScalarDataObject = CastChecked<UListDataObject_Scalar>(InOwningListDataObject);

	// 设置滑块的当前值
	CommonNumeric_SettingValue->SetNumericType(OwningScalarDataObject->GetDisplayNumericType());
	CommonNumeric_SettingValue->FormattingSpecification = OwningScalarDataObject->GetNumberFormattingOptions();
	CommonNumeric_SettingValue->SetCurrentValue(OwningScalarDataObject->GetCurrentScalarValue());

	// 设置滑块的最小值和最大值，滑动步长，当前值
	AnalogSlider_SettingSlider->SetMinValue(OwningScalarDataObject->GetDisplayValueRange().GetLowerBoundValue());
	AnalogSlider_SettingSlider->SetMaxValue(OwningScalarDataObject->GetDisplayValueRange().GetUpperBoundValue());
	AnalogSlider_SettingSlider->SetStepSize(OwningScalarDataObject->GetSliderStepSize());
	AnalogSlider_SettingSlider->SetValue(OwningScalarDataObject->GetCurrentScalarValue());

}

void UWidget_ListEntry_Scalar::OnToggleEditableState(bool bIsEditable)
{
	Super::OnToggleEditableState(bIsEditable);

	AnalogSlider_SettingSlider->SetIsEnabled(bIsEditable);
}


void UWidget_ListEntry_Scalar::OnSettingSliderValueChanged(float Value)
{
	if (OwningScalarDataObject)
	{
		// 更新当前值
		OwningScalarDataObject->SetCurrentScalarValue(Value);
	}
}

void UWidget_ListEntry_Scalar::OnSettingSliderMouseCaptureBegin()
{
	SelectThisEntryWidget();
}

void UWidget_ListEntry_Scalar::OnSettingSliderMouseCaptureEnd()
{
}

