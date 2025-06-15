// INVI_1998 All Rights Reserved.


#include "Widget/Options/DataObject/ListDataObject_String.h"

void UListDataObject_String::AddDynamicOptionsString(const FString& InStringValue, const FText& InDisplayText)
{
	AvailableOptionsStringArray.Add(InStringValue);
	AvailableOptionsTextArray.Add(InDisplayText);
	
}

void UListDataObject_String::OnDataObjectInitialized()
{
	if (!AvailableOptionsStringArray.IsEmpty())
	{
		// 如果是第一个选项，则将当前选项设置为默认选项
		CurrentStringValue = AvailableOptionsStringArray[0];
	}

	// TODO: 从设置存档中读取并设置选项

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("无效选项"));
	}
	
}

bool UListDataObject_String::TrySetDisplayTextFromStringValue(const FString& InStringValue)
{
	int32 Index = AvailableOptionsStringArray.IndexOfByKey(InStringValue);
	if (Index != INDEX_NONE)
	{
		if (AvailableOptionsTextArray.IsValidIndex(Index))
		{
			CurrentDisplayText = AvailableOptionsTextArray[Index];
			return true;
		}
	}

	return false;
}
