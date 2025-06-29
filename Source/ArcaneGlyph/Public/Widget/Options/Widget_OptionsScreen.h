// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneTypes/ArcaneEnumTypes.h"
#include "Widget/Widget_ActivatableBase.h"
#include "Widget_OptionsScreen.generated.h"

class UListDataObject_Base;
class UWidget_OptionsDetailsView;
class UFrontendCommonListView;
class UFrontendTabListWidgetBase;
class UOptionsDataRegistry;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	
	// begin UWidget interface
	virtual void NativeOnInitialized() override;
	// end UWidget interface

	// begin UCommonActivatableWidget interface
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	// end UCommonActivatableWidget interface

private:
	UOptionsDataRegistry* GetOrCreateDataRegistry();
	
	// 重设操作的行数据句柄
	UPROPERTY(EditDefaultsOnly, Category = "Frontend Options", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetActions;

	FUIActionBindingHandle ResetActionBindingHandle;	// 重设操作的绑定句柄

	// 拥有的数据注册表 通过该变量我们可以处理选项页面的数据，同时在该页面也是禁止直接访问该变量
	UPROPERTY(Transient)
	UOptionsDataRegistry* CreatedOwningDataRegistry;

	// 重设操作按钮点击事件 （由于我们不是绑定到动态多播委托，只是简单的委托绑定，所以该绑定函数不需要UFUNCTION）
	void OnResetBoundActionsTriggered();
	void OnBackBoundActionsTriggered();

	FString TryGetEntryWidgetClassNameByDataObject(UObject* Item) const;

	// ******** 绑定到选项数据注册表的控件 ********
	UPROPERTY(meta = (BindWidget))
	UFrontendTabListWidgetBase* TabListWidget_OptionsTabs;	// 选项卡列表小部件

	UPROPERTY(meta=(BindWidget))
	UFrontendCommonListView* CommonListView_OptionsList;	// 选项列表小部件

	UPROPERTY(meta = (BindWidget))
	UWidget_OptionsDetailsView* CommonDetailsView_ListDetailsView;	// 选项详情视图小部件

	// ******** 绑定到选项数据注册表的控件 ********

	void OnListViewListDataModified(UListDataObject_Base* ModifyData, EOptionsListDataModifyReason OptionsListDataModifyReason);
	
	UFUNCTION()
	void OnOptionsTabSelected(FName InTabId);

	void OnListViewItemHovered(UObject* Item, bool bIsHovered);
	void OnListViewItemSelectionChanged(UObject* Item);

	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ResetTableDataArray;	// 重设表格数据数组，用于存储当前选项列表中的数据对象
};
