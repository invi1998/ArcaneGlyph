// INVI_1998 All Rights Reserved.


#include "Widget/Options/Widget_OptionsDetailsView.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "CommonRichTextBlock.h"
#include "Widget/Options/DataObject/ListDataObject_Base.h"

void UWidget_OptionsDetailsView::UpdateDetailsViewInfo(UListDataObject_Base* InListDataObject, const FString& InEntryWidgetClassName)
{
	if (!InListDataObject || InEntryWidgetClassName.IsEmpty())
	{
		ClearDetailsViewInfo();
		return;
	}
	if (CommonTextBlock_OptionsDetailsTitle)
	{
		CommonTextBlock_OptionsDetailsTitle->SetText(InListDataObject->GetDataDisplayName());
	}
	if (CommonLazyImage_OptionsDetailsImage)
	{
		if (!InListDataObject->GetSoftDescriptionImage().IsNull())
		{
			CommonLazyImage_OptionsDetailsImage->SetBrushFromLazyTexture(InListDataObject->GetSoftDescriptionImage());
			CommonLazyImage_OptionsDetailsImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);		// 显示图片，但是不遮挡鼠标点击事件
		}
	}
	if (CommonRichTextBlock_Description)
	{
		CommonRichTextBlock_Description->SetText(InListDataObject->GetDataDescriptionRichText());
	}
	
	if (CommonRichTextBlock_DynamicDetails)
	{
		const FString DynamicDetailsText = FString::Printf(TEXT("数据对象类型: <Dynamic>%s</>\n数据ID: <Dynamic>%s</>\n数据类型控件名: <Dynamic>%s</>"),
			*InListDataObject->GetClass()->GetName(),
			*InListDataObject->GetDataID().ToString(),
			*InEntryWidgetClassName
		);
		CommonRichTextBlock_DynamicDetails->SetText(FText::FromString(DynamicDetailsText));
	}

	if (CommonRichTextBlock_AdditionalDetails)
	{
		CommonRichTextBlock_AdditionalDetails->SetText(InListDataObject->GetDisabledRichText());
	}
}

void UWidget_OptionsDetailsView::ClearDetailsViewInfo()
{
	if (CommonTextBlock_OptionsDetailsTitle) 
	{
		CommonTextBlock_OptionsDetailsTitle->SetText(FText::GetEmpty());
	}
	if (CommonLazyImage_OptionsDetailsImage) 
	{
		CommonLazyImage_OptionsDetailsImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (CommonRichTextBlock_Description) 
	{
		CommonRichTextBlock_Description->SetText(FText::GetEmpty());
	}
	if (CommonRichTextBlock_DynamicDetails)
	{
		CommonRichTextBlock_DynamicDetails->SetText(FText::GetEmpty());
	}
	if (CommonRichTextBlock_AdditionalDetails)
	{
		CommonRichTextBlock_AdditionalDetails->SetText(FText::GetEmpty());
	}
}

void UWidget_OptionsDetailsView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ClearDetailsViewInfo();
}
