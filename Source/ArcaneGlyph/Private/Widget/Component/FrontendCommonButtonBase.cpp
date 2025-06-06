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

void UFrontendCommonButtonBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetButtenText(ButtonDisplayText);
}
