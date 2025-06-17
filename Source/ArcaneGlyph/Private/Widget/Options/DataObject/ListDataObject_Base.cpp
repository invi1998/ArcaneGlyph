// INVI_1998 All Rights Reserved.


#include "Widget/Options/DataObject/ListDataObject_Base.h"

#include "FrontendSettings/FrontendGameUserSettings.h"

void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* InListData, EOptionsListDataModifyReason InModifyReason)
{
	OnListDataModified.Broadcast(InListData, InModifyReason);

	if (bShouldApplyChangeImmediately)
	{
		UFrontendGameUserSettings::Get()->ApplySettings(true); // 立即应用设置
	}
}
