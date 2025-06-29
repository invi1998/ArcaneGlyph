// INVI_1998 All Rights Reserved.


#include "Widget/Options/Widget_OptionsScreen.h"

#include "ArcaneDebugHelper.h"
#include "ICommonInputModule.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Input/CommonUIInputTypes.h"
#include "Widget/Component/FrontendCommonListView.h"
#include "Widget/Component/FrontendTabListWidgetBase.h"
#include "Widget/Options/OptionsDataRegistry.h"
#include "Widget/Options/Widget_OptionsDetailsView.h"
#include "Widget/Options/DataObject/ListDataObject_Collection.h"
#include "Widget/Options/ListEntries/Widget_ListEntry_Base.h"

void UWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetActions.IsNull())
	{
		// 向绑定操作栏插入自定义绑定操作并保存绑定句柄
		ResetActionBindingHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				ResetActions,
				true, // 在操作栏中显示
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionsTriggered)
			)
		);
	}

	// 完成自定义绑定后，我们可以注册默认的返回操作
	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true, // 在操作栏中显示
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionsTriggered)
		)
	);

	TabListWidget_OptionsTabs->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnOptionsTabSelected);

	// 绑定选项列表视图的项悬停事件，选中事件
	CommonListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::OnListViewItemHovered);
	CommonListView_OptionsList->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnListViewItemSelectionChanged);
}

void UWidget_OptionsScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (UListDataObject_Collection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollections())
	{
		if (!TabCollection) continue;

		const FName TabID = TabCollection->GetDataID();
		if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID))
		{
			// 如果已存在具有此 ID 的按钮，则意味着该按钮之前已被创建过。
			continue;
		}

		TabListWidget_OptionsTabs->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
	}
}

void UWidget_OptionsScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	// 在选项界面被关闭时，保存当前页面的设置
	UFrontendGameUserSettings::Get()->ApplySettings(true); // true表示保存设置并应用
}

UOptionsDataRegistry* UWidget_OptionsScreen::GetOrCreateDataRegistry()
{
	if (!CreatedOwningDataRegistry)
	{
		CreatedOwningDataRegistry = NewObject<UOptionsDataRegistry>(this, UOptionsDataRegistry::StaticClass());

		CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}

	checkf(CreatedOwningDataRegistry, TEXT("UWidget_OptionsScreen::GetOrCreateDataRegistry - CreatedOwningDataRegistry is null!"));

	return CreatedOwningDataRegistry;
}

void UWidget_OptionsScreen::OnResetBoundActionsTriggered()
{
	Debug::Print(TEXT("Reset Bound Actions Triggered!"), FColor::Green);
}

void UWidget_OptionsScreen::OnBackBoundActionsTriggered()
{
	// 返回操作触发时，通常会关闭当前选项界面
	DeactivateWidget();
}

FString UWidget_OptionsScreen::TryGetEntryWidgetClassNameByDataObject(UObject* Item) const
{
	UUserWidget* EntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(Item);
	if (!EntryWidget)
	{
		return FString();
	}
	return EntryWidget->GetClass()->GetName();
}

void UWidget_OptionsScreen::OnListViewListDataModified(UListDataObject_Base* ModifyData, EOptionsListDataModifyReason OptionsListDataModifyReason)
{
	// 当列表数据被修改时，更新默认值
	if (!ModifyData) return;

	if (ModifyData->CanResetToDefault())
	{
		ResetTableDataArray.AddUnique(ModifyData);
		if (!GetActionBindings().Contains(ResetActionBindingHandle))
		{
			// 如果重设操作的绑定句柄不在当前操作绑定列表中，则添加它
			AddActionBinding(ResetActionBindingHandle);
		}
	}
	else
	{
		if (ResetTableDataArray.Contains(ModifyData))
		{
			// 如果该项已经在重设表格数据数组中，则将其移除
			ResetTableDataArray.Remove(ModifyData);
		}
	}

	// 如果重设表格数据数组为空，则移除重设操作的绑定
	if (ResetTableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionBindingHandle);
	}
}

void UWidget_OptionsScreen::OnOptionsTabSelected(FName InTabId)
{
	CommonDetailsView_ListDetailsView->ClearDetailsViewInfo();
	
	const TArray<UListDataObject_Base*> FoundListSourceItems = GetOrCreateDataRegistry()->GetListSourceItemBySelectedTabID(InTabId);

	CommonListView_OptionsList->SetListItems(FoundListSourceItems);
	CommonListView_OptionsList->RequestRefresh();

	if (CommonListView_OptionsList->GetNumItems() > 0)
	{
		// 如果列表中有项，则导航到第一个项
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}

	ResetTableDataArray.Empty();

	for (UListDataObject_Base* FoundListSourceItem : FoundListSourceItems)
	{
		if (!FoundListSourceItem)
		{
			continue; // 如果项为空，则跳过
		}

		if (!FoundListSourceItem->OnListDataModified.IsBoundToObject(this))
		{
			// 如果该项的OnListDataModified事件已经绑定到当前对象，则不需要重复绑定，没有的话就进行绑定
			FoundListSourceItem->OnListDataModified.AddUObject(this, &ThisClass::OnListViewListDataModified);
		}
		
		if (FoundListSourceItem->CanResetToDefault())
		{
			// 如果该项有默认值并且可以重置到默认值，则将其添加到重设表格数据数组中
			// 这样我们就可以在需要时重置这些项
			ResetTableDataArray.Add(FoundListSourceItem);
		}
	}

	// 将页面的重置为默认选项的按钮打开
	if (ResetTableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionBindingHandle);
	}
	else
	{
		if (!GetActionBindings().Contains(ResetActionBindingHandle))
		{
			// 如果重设操作的绑定句柄不在当前操作绑定列表中，则添加它
			AddActionBinding(ResetActionBindingHandle);
		}
	}
	
}

void UWidget_OptionsScreen::OnListViewItemHovered(UObject* Item, bool bIsHovered)
{
	if (!Item) return;

	UWidget_ListEntry_Base* HoveredEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(Item);
	if (!HoveredEntryWidget) return;

	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bIsHovered);

	if (bIsHovered)
	{
		// 如果鼠标悬停在该项上，则选中该项
		CommonDetailsView_ListDetailsView->UpdateDetailsViewInfo(
			CastChecked<UListDataObject_Base>(Item),
			TryGetEntryWidgetClassNameByDataObject(Item)
		);
	}
	else
	{
		// 鼠标离开时取消选中
		if (UListDataObject_Base* SelectedItem = CommonListView_OptionsList->GetSelectedItem<UListDataObject_Base>())
		{
			CommonDetailsView_ListDetailsView->UpdateDetailsViewInfo(SelectedItem, TryGetEntryWidgetClassNameByDataObject(SelectedItem));
		}
		else
		{
			CommonDetailsView_ListDetailsView->ClearDetailsViewInfo();
		}
	}
}

void UWidget_OptionsScreen::OnListViewItemSelectionChanged(UObject* Item)
{
	if (!Item) return;

	CommonDetailsView_ListDetailsView->UpdateDetailsViewInfo(
			CastChecked<UListDataObject_Base>(Item),
			TryGetEntryWidgetClassNameByDataObject(Item)
		);
}
