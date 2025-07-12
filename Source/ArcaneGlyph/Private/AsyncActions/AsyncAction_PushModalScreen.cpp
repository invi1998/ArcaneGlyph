// INVI_1998 All Rights Reserved.


#include "AsyncActions/AsyncAction_PushModalScreen.h"

#include "Subsystems/FrontendUISubsystem.h"

UAsyncAction_PushModalScreen* UAsyncAction_PushModalScreen::PushModalScreen(const UObject* WorldContextObject,
	UPARAM(meta = (Categories = "Frontend.Widget.ModalScreen")) FGameplayTag WidgetStackTag,
	EModalType ModalType, FText ModalTitle, FText ModalSubtitle, FText ModalMessage, FText ModalDescription,
	FSlateBrush ModalIcon, EColorThemeType ModalTheme,
	FText ConfirmButtonText, FText CancelButtonText, FText CloseButtonText)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushModalScreen* Action = NewObject<UAsyncAction_PushModalScreen>();
			Action->CachedOwingWorld = World;
			Action->CachedWidgetStackTag = WidgetStackTag;
			Action->CachedModalType = ModalType;
			Action->CachedModalTitle = ModalTitle;
			Action->CachedModalSubtitle = ModalSubtitle;
			Action->CachedModalMessage = ModalMessage;
			Action->CachedModalDescription = ModalDescription;
			Action->CachedModalIcon = ModalIcon;
			Action->CachedModalTheme = ModalTheme;
			Action->CachedConfirmButtonText = ConfirmButtonText;
			Action->CachedCancelButtonText = CancelButtonText;
			Action->CachedCloseButtonText = CloseButtonText;

			Action->RegisterWithGameInstance(World);

			return Action;
		}
	}
	return nullptr;
}

void UAsyncAction_PushModalScreen::Activate()
{
	UFrontendUISubsystem::Get(CachedOwingWorld.Get())->PushModalScreenToModalStackAsync(
		CachedWidgetStackTag, CachedModalType, CachedModalTitle, CachedModalSubtitle, CachedModalMessage, CachedModalDescription, CachedModalIcon,
		[this](EModalButtonType ButtonType)
		{
			OnModalScreenButtonClicked.Broadcast(ButtonType);

			SetReadyToDestroy();
		},
		CachedModalTheme, CachedConfirmButtonText, CachedCancelButtonText, CachedCloseButtonText
	);
}
