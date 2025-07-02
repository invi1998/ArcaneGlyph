// INVI_1998 All Rights Reserved.


#include "Widget/Options/DataObject/ListDataObject_String.h"

#include "Distributions/Distribution.h"
#include "Widget/Options/OptionsDataInteractionHelper.h"

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

	// 将当前选项值设置到动态获取器或设置器中（进而保存到玩家游戏设置中GameUserSettings）
	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		NotifyListDataModified(this);
	}
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

void UListDataObject_String::OnRotatorInitiatedValueChanged(const FText& InNewValue)
{
	// 该函数在Rotator组件的值改变时被调用，用于处理手柄模式下选项的切换
	
	// 找到当前选项值在可用选项字符串数组中的索引位置
	const int32 CurrentOptionValueIndex = AvailableOptionsTextArray.IndexOfByPredicate(
		[InNewValue](const FText& InStringValue)->bool
		{
			return InStringValue.EqualTo(InNewValue);
		}
	);

	if (CurrentOptionValueIndex != INDEX_NONE && AvailableOptionsStringArray.IsValidIndex(CurrentOptionValueIndex))
	{
		// 如果找到了当前选项值，则将其设置为当前字符串值
		CurrentDisplayText = InNewValue;
		CurrentStringValue = AvailableOptionsStringArray[CurrentOptionValueIndex];

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);
			NotifyListDataModified(this);
		}
	}
}

void UListDataObject_String::OnDataObjectInitialized()
{
	if (!AvailableOptionsStringArray.IsEmpty())
	{
		// 如果是第一个选项，则将当前选项设置为默认选项
		CurrentStringValue = AvailableOptionsStringArray[0];
	}

	if (HasDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();
	}

	// 从玩家游戏设置中读取并设置选项
	if (DataDynamicGetter)
	{
		if (!DataDynamicGetter->GetValueAsString().IsEmpty())
		{
			CurrentStringValue = DataDynamicGetter->GetValueAsString();
		}
	}

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("无效选项"));
	}
	
}

bool UListDataObject_String::CanResetToDefault() const
{
	return HasDefaultValue() && CurrentStringValue != GetDefaultValueAsString();
}

bool UListDataObject_String::TryResetToDefault()
{
	if (CanResetToDefault())
	{
		CurrentStringValue = GetDefaultValueAsString();
		if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
		{
			CurrentDisplayText = FText::FromString(TEXT("无效选项"));
		}

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);
			NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);
		}
		
		return true;
	}
	return false;
	
}

bool UListDataObject_String::CanSetToForcedStringValue(const FString& InForcedStringValue) const
{
	return AvailableOptionsStringArray.Contains(InForcedStringValue) && CurrentStringValue != InForcedStringValue;
}

void UListDataObject_String::OnSetToForcedStringValue(const FString& InForcedStringValue)
{
	CurrentStringValue = InForcedStringValue;
	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		NotifyListDataModified(this, EOptionsListDataModifyReason::DependencyModified);
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

// *********************** String Bool Start ***********************

void UListDataObject_StringBool::OverrideDisplayTrueText(const FText& InDisplayText)
{
	if (!AvailableOptionsStringArray.Contains(TrueString))
	{
		AddDynamicOptionsString(TrueString, InDisplayText);
	}
}

void UListDataObject_StringBool::OverrideDisplayFalseText(const FText& InDisplayText)
{
	if (!AvailableOptionsStringArray.Contains(FalseString))
	{
		AddDynamicOptionsString(FalseString, InDisplayText);
	}
}

void UListDataObject_StringBool::SetTrueAsDefaultValue()
{
	SetDefaultValueFromString(TrueString);
}

void UListDataObject_StringBool::SetFalseAsDefaultValue()
{
	SetDefaultValueFromString(FalseString);
}

void UListDataObject_StringBool::OnDataObjectInitialized()
{
	TryInitializeBoolValue();
	Super::OnDataObjectInitialized();
}

void UListDataObject_StringBool::TryInitializeBoolValue()
{
	if (!AvailableOptionsStringArray.Contains(TrueString))
	{
		AddDynamicOptionsString(TrueString, FText::FromString(TEXT("启用")));
	}

	if (!AvailableOptionsStringArray.Contains(FalseString))
	{
		AddDynamicOptionsString(FalseString, FText::FromString(TEXT("关闭")));
	}
}

// *********************** String Bool End ***********************
