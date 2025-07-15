// INVI_1998 All Rights Reserved.


#include "Subsystems/FrontendUISubsystem.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneGameplayTags.h"
#include "Engine/AssetManager.h"
#include "Widget/Widget_ActivatableBase.h"
#include "Widget/Widget_ModalScreen.h"
#include "Widget/Widget_PrimaryLayout.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

UFrontendUISubsystem* UFrontendUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UFrontendUISubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

bool UFrontendUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		// 如果不是专用服务器实例，就查找是否有派生类，如果没有派生类，则不创建子系统
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	// 如果是专用服务器实例，则不创建子系统（因为服务器不需要UI）
	return false;
}

void UFrontendUISubsystem::RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InWidget)
{
	check(InWidget);
	CreatedPrimaryLayoutWidget = InWidget;
}

void UFrontendUISubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& WidgetTag, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallback)
{
	check(!InSoftWidgetClass.IsNull());

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		InSoftWidgetClass.ToSoftObjectPath(),
		// 使用移动语义，以避免不必要的拷贝，将回调传递给异步加载操作
		FStreamableDelegate::CreateLambda([this, WidgetTag, InSoftWidgetClass, AsyncPushStateCallback]()
		{
			// 确保主布局小部件已创建
			UClass* LoadedWidgetClass = InSoftWidgetClass.Get();
			check(LoadedWidgetClass && CreatedPrimaryLayoutWidget);

			if (UCommonActivatableWidgetContainerBase* WidgetStack = CreatedPrimaryLayoutWidget->FindWidgetStackByTag(WidgetTag))
			{
				UWidget_ActivatableBase* CreateWidget = WidgetStack->AddWidget<UWidget_ActivatableBase>(
					LoadedWidgetClass,
					// 绑定异步推送状态回调 (推送到控件栈之前的回调绑定）
					[AsyncPushStateCallback](UWidget_ActivatableBase& NewWidget)
					{
						// 调用异步推送状态回调
						AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &NewWidget);
					}
				);

				// 调用异步推送状态回调 (推送到控件栈之后的回调绑定）
				AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreateWidget);
				
			}

		})
	);
}

void UFrontendUISubsystem::PushModalScreenToModalStackAsync(const FGameplayTag& WidgetTag, EModalType ModalType, const FText& ModalTitle,
	const FText& ModalSubtitle, const FText& ModalMessage, const FText& ModalDescription, const FSlateBrush& ModalIcon,
	TFunction<void(EModalButtonType)> ButtonClickedCallback, EColorThemeType InColorTheme,
	const FText& ConfirmButtonText, const FText& CancelButtonText, const FText& CloseButtonText)
{
	UConfirmScreenInfoObject* ConfirmScreenInfoObject = nullptr;
	switch (ModalType)
	{
	case EModalType::Ok:
		ConfirmScreenInfoObject = UConfirmScreenInfoObject::CreateOKScreen(ModalTitle, ModalSubtitle, ModalMessage, ModalDescription, ModalIcon, ConfirmButtonText);
		break;
	case EModalType::OkCancel:
		ConfirmScreenInfoObject = UConfirmScreenInfoObject::CreateOKCancelScreen(ModalTitle, ModalSubtitle, ModalMessage, ModalDescription, ModalIcon, ConfirmButtonText, CloseButtonText);
		break;
	case EModalType::YesNo:
		ConfirmScreenInfoObject = UConfirmScreenInfoObject::CreateYesNoScreen(ModalTitle, ModalSubtitle, ModalMessage, ModalDescription, ModalIcon, ConfirmButtonText, CancelButtonText);
		break;
	case EModalType::YesNoClose:
		ConfirmScreenInfoObject = UConfirmScreenInfoObject::CreateYesNoCloseScreen(ModalTitle, ModalSubtitle, ModalMessage, ModalDescription, ModalIcon, ConfirmButtonText, CancelButtonText, CloseButtonText);
		break;
	case EModalType::Unknow:
		break;
	}

	check(ConfirmScreenInfoObject);

	PushSoftWidgetToStackAsync(
		ArcaneGameplayTags::Frontend_WidgetStack_Modal,
		UArcaneBlueprintFunctionLibrary::GetFrontendSoftWidgetClassByTag(WidgetTag),
		[this, ConfirmScreenInfoObject, ButtonClickedCallback, InColorTheme](EAsyncPushWidgetState State, UWidget_ActivatableBase* PushedWidget)
		{
			if (State == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				if (UWidget_ModalScreen* ModalScreen = Cast<UWidget_ModalScreen>(PushedWidget))
				{
					ModalScreen->InitConfirmScreen(ConfirmScreenInfoObject, ButtonClickedCallback, InColorTheme);
				}
			}
		}
	);
}
