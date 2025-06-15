// INVI_1998 All Rights Reserved.


#include "Widget/Options/DataObject/ListDataObject_Base.h"

void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* InListData, EOptionsListDataModifyReason InModifyReason)
{
	OnListDataModified.Broadcast(InListData, InModifyReason);
}
