// INVI_1998 All Rights Reserved.


#include "Widget/Widget_PrimaryLayout.h"

UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindWidgetStackByTag(const FGameplayTag& WidgetTag) const
{
	checkf(RegisteredWidgetsStackMap.Contains(WidgetTag), TEXT("Widget not registered by the tag %s"), *WidgetTag.ToString());

	return RegisteredWidgetsStackMap.FindRef(WidgetTag);
}

void UWidget_PrimaryLayout::RegisterWidgetToStack(UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag WidgetTag, UCommonActivatableWidgetContainerBase* WidgetContainer)
{
	// 只有在设计时才会注册控件到栈中
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetsStackMap.Contains(WidgetTag))
		{
			RegisteredWidgetsStackMap.Add(WidgetTag, WidgetContainer);
		}
	}
}
