// INVI_1998 All Rights Reserved.


#include "Widget/Options/Widget_OptionsScreen.h"

#include "ArcaneDebugHelper.h"
#include "ICommonInputModule.h"
#include "Input/CommonUIInputTypes.h"
#include "Widget/Component/FrontendCommonListView.h"
#include "Widget/Component/FrontendTabListWidgetBase.h"
#include "Widget/Options/OptionsDataRegistry.h"
#include "Widget/Options/DataObject/ListDataObject_Collection.h"

void UWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetActions.IsNull())
	{
		// 向绑定操作栏插入自定义绑定操作并保存绑定句柄
		ResetBindingHandle = RegisterUIActionBinding(
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

void UWidget_OptionsScreen::OnOptionsTabSelected(FName InTabId)
{
	const TArray<UListDataObject_Base*> FoundListSourceItems = GetOrCreateDataRegistry()->GetListSourceItemBySelectedTabID(InTabId);

	CommonListView_OptionsList->SetListItems(FoundListSourceItems);
	CommonListView_OptionsList->RequestRefresh();

	if (CommonListView_OptionsList->GetNumItems() > 0)
	{
		// 如果列表中有项，则导航到第一个项
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}
	
}
