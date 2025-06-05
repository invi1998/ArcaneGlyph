// INVI_1998 All Rights Reserved.


#include "AsyncActions/AsyncAction_PushSoftWidget.h"

UAsyncAction_PushSoftWidget* UAsyncAction_PushSoftWidget::PushSoftWidget(const UObject* WorldContextObject, APlayerController* PlayerController, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag WidgetTag, bool bFocusOnNewPushedWidget)
{
	checkf(!InSoftWidgetClass.IsNull(), TEXT("InSoftWidgetClass cannot be null!"));

	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushSoftWidget* AsyncAction = NewObject<UAsyncAction_PushSoftWidget>();
			AsyncAction->RegisterWithGameInstance(World);

			return AsyncAction;
		}
	}

	return nullptr;
}
