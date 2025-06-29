// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_OptionsDetailsView.generated.h"

class USizeBox;
class UListDataObject_Base;
class UCommonLazyImage;
class UCommonTextBlock;
class UCommonRichTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_OptionsDetailsView : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateDetailsViewInfo(UListDataObject_Base* InListDataObject, const FString& InEntryWidgetClassName = TEXT(""));
	void ClearDetailsViewInfo();


protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_OptionsDetailsTitle;	// 选项详情标题文本块

	UPROPERTY(meta=(BindWidget))
	USizeBox* ImageSizer;	// 选项详情图片的大小盒子，用于控制图片的大小

	UPROPERTY(meta=(BindWidgetOptional))
	UCommonLazyImage* CommonLazyImage_OptionsDetailsImage;	// 选项详情图片

	UPROPERTY(meta=(BindWidgetOptional))
	UCommonRichTextBlock* CommonRichTextBlock_Description;	// 选项详情描述文本块

	UPROPERTY(meta=(BindWidgetOptional))
	UCommonRichTextBlock* CommonRichTextBlock_DynamicDetails;	// 动态详情文本块

	UPROPERTY(meta=(BindWidgetOptional))
	UCommonRichTextBlock* CommonRichTextBlock_AdditionalDetails;	// 附加详情文本块
};
