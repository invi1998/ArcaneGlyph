// INVI_1998 All Rights Reserved.


#include "Widget/Component/Widget_ListElementBase.h"

#include "CommonTextBlock.h"

void UWidget_ListElementBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetElementText(ElementText);
}

void UWidget_ListElementBase::SetElementText(const FText& InElementText)
{
	ElementText = InElementText;
	if (CommonTextBlock_ElementText)
	{
		CommonTextBlock_ElementText->SetText(ElementText);
	}
}

void UWidget_ListElementBase::SetDisabled(bool bInDisabled)
{
	if (CommonTextBlock_ElementText)
	{
		CommonTextBlock_ElementText->SetIsEnabled(!bInDisabled);
	}
}
