// INVI_1998 All Rights Reserved.


#include "Widget/Options/DataObject/ListDataObject_String.h"

void UListDataObject_String::AddDynamicOptionsString(const FString& InStringValue, const FText& InDisplayText)
{
	AvailableOptionsStringArray.Add(InStringValue);
	AvailableOptionsTextArray.Add(InDisplayText);
	
}

void UListDataObject_String::AdvanceToNextOption()
{
	if (AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty())
	{
		return; // 如果没有可用选项，则直接返回
	}

	const int32 CurrentOptionValueIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 NextOptionValueIndex = CurrentOptionValueIndex + 1;

	if (AvailableOptionsStringArray.IsValidIndex(NextOptionValueIndex))
	{
		CurrentStringValue = AvailableOptionsStringArray[NextOptionValueIndex];
	}
	else
	{
		CurrentStringValue = AvailableOptionsStringArray[0]; // 如果下一个索引无效，则回到第一个选项
	}

	TrySetDisplayTextFromStringValue(CurrentStringValue);

	NotifyListDataModified(this);
}

void UListDataObject_String::AdvanceToPreviousOption()
{
	if (AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty()) return;

	const int32 CurrentOptionValueIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 PreviousOptionValueIndex = CurrentOptionValueIndex - 1;
	
	if (AvailableOptionsStringArray.IsValidIndex(PreviousOptionValueIndex))
	{
		CurrentStringValue = AvailableOptionsStringArray[PreviousOptionValueIndex];
	}
	else
	{
		CurrentStringValue = AvailableOptionsStringArray.Last(); // 如果上一个索引无效，则回到最后一个选项
	}

	TrySetDisplayTextFromStringValue(CurrentStringValue);

	NotifyListDataModified(this);
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
