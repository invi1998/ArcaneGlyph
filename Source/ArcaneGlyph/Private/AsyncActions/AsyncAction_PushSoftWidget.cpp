// INVI_1998 All Rights Reserved.


#include "AsyncActions/AsyncAction_PushSoftWidget.h"

#include "Subsystems/FrontendUISubsystem.h"
#include "Widget/Widget_ActivatableBase.h"

UAsyncAction_PushSoftWidget* UAsyncAction_PushSoftWidget::PushSoftWidget(const UObject* WorldContextObject, APlayerController* PlayerController, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag WidgetStackTag, bool bFocusOnNewPushedWidget)
{
	checkf(!InSoftWidgetClass.IsNull(), TEXT("InSoftWidgetClass cannot be null!"));

	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushSoftWidget* AsyncAction = NewObject<UAsyncAction_PushSoftWidget>();
			AsyncAction->CachedOwingWorld = World;
			AsyncAction->CachedPlayerController = PlayerController;
			AsyncAction->CachedSoftWidgetClass = InSoftWidgetClass;
			AsyncAction->CachedWidgetTag = WidgetStackTag;
			AsyncAction->bFocusOnWidget = bFocusOnNewPushedWidget;
			
			AsyncAction->RegisterWithGameInstance(World);

			return AsyncAction;
		}
	}

	return nullptr;
}

void UAsyncAction_PushSoftWidget::Activate()
{
	if (!CachedOwingWorld.IsValid() || !CachedPlayerController.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AsyncAction_PushSoftWidget: Invalid world or player controller!"));
		SetReadyToDestroy();
		return;
	}
	
	if (UFrontendUISubsystem* UISubsystem = UFrontendUISubsystem::Get(CachedOwingWorld.Get()))
	{
		UISubsystem->PushSoftWidgetToStackAsync(
			CachedWidgetTag,
			CachedSoftWidgetClass,
			[this](EAsyncPushWidgetState State, UWidget_ActivatableBase* PushedWidget)
			{
				if (State == EAsyncPushWidgetState::OnCreatedBeforePush)
				{
					PushedWidget->SetOwningPlayer(CachedPlayerController.Get());
					OnWidgetCreatedBeforePush.Broadcast(PushedWidget);
				}
				else if (State == EAsyncPushWidgetState::AfterPush)
				{
					OnWidgetAfterPush.Broadcast(PushedWidget);
					if (bFocusOnWidget)
					{
						if ( UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
						{
							WidgetToFocus->SetFocus();	
						}
					}

					// 销毁异步操作对象
					SetReadyToDestroy();
				}
			});
	}
}
