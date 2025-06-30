// INVI_1998 All Rights Reserved.


#include "Widget/Options/DataObject/ListDataObject_Scalar.h"

#include "Widget/Options/OptionsDataInteractionHelper.h"

FCommonNumberFormattingOptions UListDataObject_Scalar::NoDecimal()
{
	FCommonNumberFormattingOptions Options;
	Options.MinimumFractionalDigits = 0;
	Options.MaximumFractionalDigits = 0;
	Options.RoundingMode = ERoundingMode::HalfToEven; // 四舍五入到最接近的偶数

	return Options;
}

FCommonNumberFormattingOptions UListDataObject_Scalar::WithDecimal(int32 NumDecimalPlaces)
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = NumDecimalPlaces;
	Options.RoundingMode = ERoundingMode::HalfToEven;

	return Options;
}

float UListDataObject_Scalar::GetCurrentScalarValue() const
{
	if (DataDynamicGetter)
	{
		// 如果有动态获取器，则从动态获取器中获取当前值
		// 然后将其映射到显示范围内
		return FMath::GetMappedRangeValueClamped(
			OutputValueRange,
			DisplayValueRange,
			StringToFloat(DataDynamicGetter->GetValueAsString())
		);
	}

	return 0.f;
}

void UListDataObject_Scalar::SetCurrentScalarValue(float Value)
{
	if (DataDynamicSetter)
	{
		// 如果有动态设置器，则将当前值设置到动态设置器中
		// 这里需要将值映射到输出范围内
		const float MappedValue = FMath::GetMappedRangeValueClamped(
			DisplayValueRange,
			OutputValueRange,
			Value
		);

		NotifyListDataModified(this);
		
		DataDynamicSetter->SetValueFromString(LexToString(MappedValue));
		
	}
}

float UListDataObject_Scalar::StringToFloat(const FString& InString)
{
	float OutFloat = 0.0f;
	LexFromString(OutFloat, InString);
	return OutFloat;
}
