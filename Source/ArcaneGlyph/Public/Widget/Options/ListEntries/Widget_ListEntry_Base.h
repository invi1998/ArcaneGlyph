// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Widget_ListEntry_Base.generated.h"

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

protected:
	// Begin IUserObjectListEntry interface
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// End IUserObjectListEntry interface


private:
	// **** Bound Widgets **** //
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonTextBlock_SettingDisplayName;	// 设置显示名称文本块
	// **** Bound Widgets **** //
	
};
