// INVI_1998 All Rights Reserved.


#include "Widget/Options/DataObject/ListDataObject_Collection.h"

TArray<UListDataObject_Base*> UListDataObject_Collection::GetAllChildListData() const
{
	return ChildListDataArray;
}

bool UListDataObject_Collection::HasAnyChildListData() const
{
	return !ChildListDataArray.IsEmpty();
}

void UListDataObject_Collection::AddChildListData(UListDataObject_Base* InChildListData)
{
	// 通知子列表数据执行初始化
	InChildListData->InitDataObject();

	// 将子列表的父节点设置为this（当前对象节点）
	InChildListData->SetParentData(this);
	
	ChildListDataArray.Add(InChildListData);
}
