// INVI_1998 All Rights Reserved.


#include "Widget/Component/FrontendCommonGridButtonBase.h"

#include "CommonTextBlock.h"
#include "Components/VerticalBox.h"
#include "Widget/Component/Widget_ListElementBase.h"

void UFrontendCommonGridButtonBase::SetCategoryText(FText InCategoryText)
{
	CategoryText = InCategoryText;
	if (GridButton_CategoryText)
	{
		GridButton_CategoryText->SetText(CategoryText);
	}
}

void UFrontendCommonGridButtonBase::SetTitleText(FText InTitleText)
{
	TitleText = InTitleText;
	if (GridButton_TitleText)
	{
		GridButton_TitleText->SetText(TitleText);
	}
}

void UFrontendCommonGridButtonBase::SetSubTitleText(FText InSubTitleText)
{
	SubTitleText = InSubTitleText;
	if (GridButton_SubTitleText)
	{
		GridButton_SubTitleText->SetText(SubTitleText);
	}
}

void UFrontendCommonGridButtonBase::SetDescriptionElementList(TArray<FText> InDescriptionLists)
{
	DescriptionLists = InDescriptionLists;
	if (GridButton_SubDescriptionVerticalBox && DescriptionElementWidgetClass)
	{
		// 清空现有的描述元素列表
		GridButton_SubDescriptionVerticalBox->ClearChildren();

		for (const FText& DescriptionText : DescriptionLists)
		{
			if (UWidget_ListElementBase* NewElement = CreateWidget<UWidget_ListElementBase>(this, DescriptionElementWidgetClass))
			{
				NewElement->SetElementText(DescriptionText);
				GridButton_SubDescriptionVerticalBox->AddChild(NewElement);
			}
		}
	}
	
}

void UFrontendCommonGridButtonBase::ToggleGridButtonLock(bool bLock)
{
	bGridButtonLocked = bLock;
	if (bGridButtonLocked)
	{
		SetIsEnabled(false);
	}
	else
	{
		SetIsEnabled(true);
	}
}

void UFrontendCommonGridButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetCategoryText(CategoryText);
	SetTitleText(TitleText);
	SetSubTitleText(SubTitleText);
	SetDescriptionElementList(DescriptionLists);

	ToggleGridButtonLock(bGridButtonLocked);
}

void UFrontendCommonGridButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();
	
	if (GridButton_TitleText && GetCurrentTextStyleClass())
	{
		GridButton_TitleText->SetStyle(GetCurrentTextStyleClass());
	}
	
}


