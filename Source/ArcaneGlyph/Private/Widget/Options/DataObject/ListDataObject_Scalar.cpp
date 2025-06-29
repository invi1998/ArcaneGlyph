// INVI_1998 All Rights Reserved.


#include "Widget/Options/DataObject/ListDataObject_Scalar.h"

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
