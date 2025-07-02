// INVI_1998 All Rights Reserved.

#pragma once

#include "FrontendStructTypes.generated.h"

/**
 * 描述设置选项数据编辑条件的结构体
 * 某些选项可能需要根据其他选项的值来决定是否可编辑（比如，全屏状态下，分辨率不可编辑）
 */
USTRUCT()
struct FOptionDataEditConditionDescriptor
{
	GENERATED_BODY()

public:
	void SetEditConditionFunction(const TFunction<bool()>& InEditConditionFunction)
	{
		EditConditionFunction = InEditConditionFunction;
	}

	bool IsValid() const
	{
		return EditConditionFunction != nullptr;
	}

	// 检查当前选项是否满足编辑条件
	bool IsEditConditionMet() const
	{
		if (IsValid())
		{
			return EditConditionFunction();
		}
		return true; // 如果没有设置编辑条件函数，则默认返回true
	}

	FString GetDisabledRichReason() const
	{
		return DisabledRichReason;
	}

	void SetDisabledRichReason(const FString& InDisabledRichReason)
	{
		DisabledRichReason = InDisabledRichReason;
	}

	bool HasForcedDisabledStringValue() const
	{
		return DisabledForcedStringValue.IsSet();
	}

	FString GetForcedStringValue() const 
	{
		if (DisabledForcedStringValue.IsSet())
		{
			return DisabledForcedStringValue.GetValue();
		}
		return FString();
	}

	void SetForcedStringValue(const FString& InForcedStringValue)
	{
		DisabledForcedStringValue = InForcedStringValue;
	}

private:
	TFunction<bool()> EditConditionFunction;	// 编辑条件函数，用于判断当前选项是否可编辑
	FString DisabledRichReason;		// 禁用原因的富文本描述，用于在UI中显示该选项被禁用的原因
	TOptional<FString> DisabledForcedStringValue;	// 禁用时强制使用的字符串值，如果设置了该值，则在禁用状态下会使用该值作为选项的显示值
	
};
