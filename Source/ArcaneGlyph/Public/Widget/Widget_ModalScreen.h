// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget_ActivatableBase.h"
#include "Widget_ModalScreen.generated.h"

class UDynamicEntryBox;
class UCommonTextBlock;
class UCommonLazyImage;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_ModalScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* Modal_Title;	// 模态框标题文本块（可选绑定）

	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* Modal_Subtitle;	// 模态框副标题文本块（可选绑定）

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* Modal_Message;	// 模态框消息文本块

	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* Modal_Description;	// 模态框描述文本块（可选绑定）

	UPROPERTY(meta=(BindWidgetOptional))
	UCommonLazyImage* Modal_Icon;	// 模态框图标（可选绑定）

	// 这是一个可在运行时动态插入按钮的容器控件
	UPROPERTY(meta=(BindWidget))
	UDynamicEntryBox* DynamicEntryBox_Buttons;	// 模态框按钮动态入口框
};
