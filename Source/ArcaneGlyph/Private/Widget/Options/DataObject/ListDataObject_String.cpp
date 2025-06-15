// INVI_1998 All Rights Reserved.


#include "Widget/Options/DataObject/ListDataObject_String.h"

void UListDataObject_String::AddDynamicOptionsString(const FString& InStringValue, const FText& InDisplayText)
{
	AvailableOptionsStringArray.Add(InStringValue);
	AvailableOptionsTextArray.Add(InDisplayText);
	if (AvailableOptionsStringArray.Num() == 1)
	{
		// 如果是第一个选项，则将当前选项设置为默认选项
		CurrentStringValue = AvailableOptionsStringArray[0];
	}
}
