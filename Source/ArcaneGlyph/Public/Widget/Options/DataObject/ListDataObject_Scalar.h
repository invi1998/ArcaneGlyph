// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_Value.h"
#include "CommonNumericTextBlock.h"
#include "ListDataObject_Scalar.generated.h"

/**
 * 滑条数据对象类
 */
UCLASS()
class ARCANEGLYPH_API UListDataObject_Scalar : public UListDataObject_Value
{
	GENERATED_BODY()

public:
	LIST_DATA_ACCESSOR(TRange<float>, DisplayValueRange)
	LIST_DATA_ACCESSOR(TRange<float>, OutputValueRange)
	LIST_DATA_ACCESSOR(float, SliderStepSize)
	LIST_DATA_ACCESSOR(ECommonNumericType, DisplayNumericType)
	LIST_DATA_ACCESSOR(FCommonNumberFormattingOptions, NumberFormattingOptions)
	
	static FCommonNumberFormattingOptions NoDecimal();	// 无小数点格式化选项
	static FCommonNumberFormattingOptions WithDecimal(int32 NumDecimalPlaces);	// 带小数点格式化选项，传入小数位数
	static float StringToFloat(const FString& InString);

	float GetCurrentScalarValue() const;
	void SetCurrentScalarValue(float Value);

protected:
	

private:
	TRange<float> DisplayValueRange = TRange<float>(0.0f, 1.0f);	// 显示值范围，默认从0到1
	TRange<float> OutputValueRange = TRange<float>(0.0f, 1.0f);	// 输出值范围，默认从0到1
	float SliderStepSize = 0.1f;	// 滑条步进大小，默认0.1
	ECommonNumericType DisplayNumericType = ECommonNumericType::Number;	// 显示数值类型，默认数字类型
	FCommonNumberFormattingOptions NumberFormattingOptions;	// 数字格式化选项，用于控制数字的显示格式
	
};

