// INVI_1998 All Rights Reserved.


#include "Widget/Component/FrontendCommonListView.h"

#include "Editor/WidgetCompilerLog.h"
#include "Widget/Options/DataAsset_DataListEntryMapping.h"
#include "Widget/Options/DataObject/ListDataObject_Base.h"
#include "Widget/Options/DataObject/ListDataObject_Collection.h"
#include "Widget/Options/ListEntries/Widget_ListEntry_Base.h"

bool UFrontendCommonListView::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	// 如果是类别（Collection）数据对象，则不允许选择或导航
	return !FirstSelectedItem->IsA<UListDataObject_Collection>();
}

UUserWidget& UFrontendCommonListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (IsDesignTime())
	{
		// 如果是编辑状态下，就走原始父类的实现，生成并返回我们在编辑器里添加的默认widget类控件
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}

	// 我们自己的ListView实现
	// 因为原来的ListView他的Item只有一种控件类型，如果要实现多种数据类型的展示，就需要多个ListView或者其他方法
	// 我们重写该函数，用于通过在我们的DataListEntryMapping数据资产中设定好的映射表中检索传入的Item匹配控件类型实现多种数据类型的展示
	if (const TSubclassOf<UWidget_ListEntry_Base> FoundWidgetClass = DataListEntryMapping->FindEntryWidgetClassByDataObject(CastChecked<UListDataObject_Base>(Item)))
	{
		return GenerateTypedEntry<UWidget_ListEntry_Base>(FoundWidgetClass, OwnerTable);
	}

	return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}

#if WITH_EDITOR
void UFrontendCommonListView::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	// 如果 DataListEntryMapping 为空，则输出警告
	if (!DataListEntryMapping)
	{
		CompileLog.Error(
			FText::FromString(TEXT("DataListEntryMapping is not set! Please set it in the editor or in the constructor.")
			+ GetClass()->GetName() + TEXT(" is not valid!"))
		);
	}
	
}
#endif
