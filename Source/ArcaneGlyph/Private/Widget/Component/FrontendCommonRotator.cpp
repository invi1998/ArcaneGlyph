// INVI_1998 All Rights Reserved.


#include "Widget/Component/FrontendCommonRotator.h"

#include "CommonTextBlock.h"

void UFrontendCommonRotator::SetSelectedOptionByText(const FText& InOptionText)
{
	const int32 FoundIndex = TextLabels.IndexOfByPredicate(
		[InOptionText](const FText& TextItem) -> bool
		{
			return TextItem.EqualTo(InOptionText);
		}
	);

	if (FoundIndex != INDEX_NONE)
	{
		SetSelectedItem(FoundIndex);
	}
	else
	{
		MyText->SetText(InOptionText);
	}
}
