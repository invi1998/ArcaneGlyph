// INVI_1998 All Rights Reserved.


#include "Widget/Component/FrontendCommonButtonBase.h"

#include "CommonTextBlock.h"

void UFrontendCommonButtonBase::SetButtenText(FText InButtonText)
{
	if (CommonTextBlock_ButtonText && !InButtonText.IsEmpty())
	{
		CommonTextBlock_ButtonText->SetText(bUserUpperCaseButtonText ? InButtonText.ToUpper() : InButtonText);
	}
}

void UFrontendCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtenText(ButtonDisplayText);
}

void UFrontendCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	// 如果 CommonTextBlock_ButtonText 有绑定，则更新文本样式
	// 这也是为什么之前如果不重写该函数会导致我们的 CommonTextBlock_ButtonText 没有正确显示文本样式的原因
	// 我们通用按钮不知道我们用的是哪个文本块
	if (CommonTextBlock_ButtonText)
	{
		CommonTextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
	}
}

