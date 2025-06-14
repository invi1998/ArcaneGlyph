// INVI_1998 All Rights Reserved.


#include "Widget/Options/ListEntries/Widget_ListEntry_Base.h"

#include "CommonTextBlock.h"
#include "Widget/Options/DataObject/ListDataObject_Base.h"

void UWidget_ListEntry_Base::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UListDataObject_Base* ListDataObject = CastChecked<UListDataObject_Base>(ListItemObject);
	OnOwningListDataObjectSet(ListDataObject);
	
}

void UWidget_ListEntry_Base::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	if (CommonTextBlock_SettingDisplayName)
	{
		CommonTextBlock_SettingDisplayName->SetText(InOwningListDataObject->GetDataDisplayName());
	}
}
