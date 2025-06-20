// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ArcaneTypes/ArcaneEnumTypes.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Widget_ListEntry_Base.generated.h"

class UListDataObject_Base;
class UCommonTextBlock;
/**
 * 基础列表项小部件类
 *
 * 在使用CommonListView时，CommonListView需要指定EntryWidgetClass - UListViewBase需要它才能正常工作。
 * UCommonUserWidget需要实现IUserObjectListEntry接口才能被CommonListView识别使用
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_ListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnListEntryWidgetHovered"))
	void BP_OnListEntryWidgetHovered(bool bIsHovered, bool bIsEntryWidgetStillSelected);
	
	void NativeOnListEntryWidgetHovered(bool bIsHovered);

protected:
	// Begin IUserObjectListEntry interface
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// End IUserObjectListEntry interface

	// 子类应当重写此函数，以便在数据对象更新后同步 UI 数值
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* InListDataObject, EOptionsListDataModifyReason InOptionsListDataModifyReason);

	// 设置拥有的列表数据对象，在父类中，这里只被用于设置设置选项的显示名称文本，在子类中可以重写该函数来处理更多的逻辑（选项的选中文本等）
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject);

	void SelectThisEntryWidget();


private:
	// **** Bound Widgets **** //
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonTextBlock_SettingDisplayName;	// 设置显示名称文本块
	// **** Bound Widgets **** //
	
};
