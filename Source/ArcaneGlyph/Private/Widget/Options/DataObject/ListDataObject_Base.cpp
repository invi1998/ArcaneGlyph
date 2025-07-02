// INVI_1998 All Rights Reserved.


#include "Widget/Options/DataObject/ListDataObject_Base.h"

#include "FrontendSettings/FrontendGameUserSettings.h"

void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObject_Base::AddEditCondition(const FOptionDataEditConditionDescriptor& InEditCondition)
{
	EditConditions.Add(InEditCondition);
}

bool UListDataObject_Base::IsDataCurrentlyEditable()
{
	bool bIsDataCurrentlyEditable = true;
	FString CachedDisabledRichReason;
	
	for (const FOptionDataEditConditionDescriptor& Condition : EditConditions)
	{
		if (!Condition.IsEditConditionMet())
		{
			bIsDataCurrentlyEditable = false;

			CachedDisabledRichReason.Append(Condition.GetDisabledRichReason());

			SetDisabledRichText(FText::FromString(CachedDisabledRichReason));

			if (Condition.HasForcedDisabledStringValue())
			{
				const FString ForcedDisabledStringValue = Condition.GetForcedStringValue();

				// 判断当前数据项能否被设置为强制禁用的字符串值
				if (CanSetToForcedStringValue(ForcedDisabledStringValue))
				{
					OnSetToForcedStringValue(ForcedDisabledStringValue);
				}
			}
		}
	}

	return bIsDataCurrentlyEditable;
}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* InListData, EOptionsListDataModifyReason InModifyReason)
{
	OnListDataModified.Broadcast(InListData, InModifyReason);

	if (bShouldApplyChangeImmediately)
	{
		UFrontendGameUserSettings::Get()->ApplySettings(true); // 立即应用设置
	}
}
