// INVI_1998 All Rights Reserved.


#include "Widget/Component/FrontendCommonButtonBase.h"

#include "CommonTextBlock.h"
#include "Subsystems/FrontendUISubsystem.h"

void UFrontendCommonButtonBase::SetButtenText(FText InButtonText)
{
	if (CommonTextBlock_ButtonText && !InButtonText.IsEmpty())
	{
		CommonTextBlock_ButtonText->SetText(bUserUpperCaseButtonText ? InButtonText.ToUpper() : InButtonText);
	}
}

FText UFrontendCommonButtonBase::GetButtonDisplayText() const
{
	if (CommonTextBlock_ButtonText)
	{
		return CommonTextBlock_ButtonText->GetText();
	}

	return FText::GetEmpty();
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
	if (CommonTextBlock_ButtonText && GetCurrentTextStyleClass())
	{
		CommonTextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
	}
}

void UFrontendCommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (!ButtonDescriptionText.IsEmpty())
	{
		UFrontendUISubsystem::Get(this)->OnButtonDescriptionTextUpdate.Broadcast(this, ButtonDescriptionText);
	}
}

void UFrontendCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	UFrontendUISubsystem::Get(this)->OnButtonDescriptionTextUpdate.Broadcast(this, FText::GetEmpty());
}

