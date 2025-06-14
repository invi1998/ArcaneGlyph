// INVI_1998 All Rights Reserved.


#include "Widget/Options/DataAsset_DataListEntryMapping.h"

#include "Widget/Options/DataObject/ListDataObject_Base.h"

TSubclassOf<UWidget_ListEntry_Base> UDataAsset_DataListEntryMapping::FindEntryWidgetClassByDataObject(UListDataObject_Base* InDataObject) const
{
	check(InDataObject);
	
	// 这里我们需要回溯遍历InDataObject的父类，来检索列表数据类型和Widget的映射表，看是否有该类型数据对应的匹配控件项，如果存在则返回匹配结果
	// 这里最关键的地方在于我们使用传入对象的父类类型进行检索（GetSuperClass），如果不匹配就继续回溯他的父类，直到我们匹配成功
	// 这样设计是因为：比如我们的UListDataObject_String类型的数据，我们使用Widget_ListEntry_String控件类型来进行展示，
	// 在后续开发中，我们可以对UListDataObject_String进行扩展，比如 UListDataObject_StringBool，UListDataObject_StringEnum等，
	// 这些类型都可以继续使用Widget_ListEntry_String控件来进行展示，而我们不必对这些子类类型继续往我们的Mapping中添加映射关系，所以我们需要回溯检索父类来找到匹配的控件类型。
	for (UClass* DataObjectClass = InDataObject->GetClass(); DataObjectClass; DataObjectClass = DataObjectClass->GetSuperClass())
	{
		if (TSubclassOf<UListDataObject_Base> ConvertedDataObjectClass = TSubclassOf<UListDataObject_Base>(DataObjectClass))
		{
			if (DataListEntryMapping.Contains(ConvertedDataObjectClass))
			{
				return DataListEntryMapping.FindRef(ConvertedDataObjectClass);
			}
		}
	}

	return TSubclassOf<UWidget_ListEntry_Base>();	// 如果没有找到匹配的控件类型，则返回空
}
