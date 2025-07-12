// INVI_1998 All Rights Reserved.


#include "Widget/Options/ListEntries/Widget_ListEntry_KeyRemap.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneGameplayTags.h"
#include "CommonTextBlock.h"
#include "Subsystems/FrontendUISubsystem.h"
#include "Widget/Component/FrontendCommonButtonBase.h"
#include "Widget/Options/Widget_KeyRemapScreen.h"
#include "Widget/Options/DataObject/ListDataObject_KeyRemap.h"

void UWidget_ListEntry_KeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CommonButton_TriggerKey)
	{
		CommonButton_TriggerKey->SetIsEnabled(false); // 禁用按钮，因为触发按钮只是作为展示
	}
	if (CommonTextBlock_ConnectedKey)
	{
		CommonTextBlock_ConnectedKey->SetIsEnabled(false); // 禁用文本块，因为它只是作为展示
	}

	CommonButton_KeyRemap->OnClicked().AddUObject(this, &ThisClass::OnRemapKeyButtonClicked);
	CommonButton_ResetKeyBinding->OnClicked().AddUObject(this, &ThisClass::OnResetKeyBindingButtonClicked);
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningKeyRemapDataObject = CastChecked<UListDataObject_KeyRemap>(InOwningListDataObject);

	if (CachedOwningKeyRemapDataObject->HasTrigger)
	{
		CommonButton_TriggerKey->SetButtonDisplayIcon(CachedOwningKeyRemapDataObject->GetTriggerIconFromCurrentKey());
		CommonButton_TriggerKey->SetVisibility(ESlateVisibility::SelfHitTestInvisible); // 显示触发按钮
		CommonTextBlock_ConnectedKey->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		CommonButton_TriggerKey->SetVisibility(ESlateVisibility::Collapsed);
		CommonTextBlock_ConnectedKey->SetVisibility(ESlateVisibility::Collapsed);
	}

	CommonButton_KeyRemap->SetButtonDisplayIcon(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
	
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectModified(UListDataObject_Base* InListDataObject, EOptionsListDataModifyReason InOptionsListDataModifyReason)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CommonButton_KeyRemap->SetButtonDisplayIcon(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
		
		if (CachedOwningKeyRemapDataObject->HasTrigger)
		{
			CommonButton_TriggerKey->SetButtonDisplayIcon(CachedOwningKeyRemapDataObject->GetTriggerIconFromCurrentKey());
			CommonButton_TriggerKey->SetVisibility(ESlateVisibility::SelfHitTestInvisible); // 显示触发按钮
			CommonTextBlock_ConnectedKey->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			CommonButton_TriggerKey->SetVisibility(ESlateVisibility::Collapsed);
			CommonTextBlock_ConnectedKey->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
}

void UWidget_ListEntry_KeyRemap::OnToggleEditableState(bool bIsEditable)
{
	Super::OnToggleEditableState(bIsEditable);
	if (CommonButton_KeyRemap)
	{
		CommonButton_KeyRemap->SetIsEnabled(bIsEditable); // 设置键位重映射按钮的可用状态
	}
	if (CommonButton_ResetKeyBinding)
	{
		CommonButton_ResetKeyBinding->SetIsEnabled(bIsEditable); // 设置重置键位绑定按钮的可用状态
		CommonButton_ResetKeyBinding->SetVisibility(bIsEditable ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UWidget_ListEntry_KeyRemap::OnRemapKeyButtonClicked()
{
	UFrontendUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		ArcaneGameplayTags::Frontend_WidgetStack_Modal,
		UArcaneBlueprintFunctionLibrary::GetFrontendSoftWidgetClassByTag(ArcaneGameplayTags::Frontend_Widget_ModalScreen_KeyRemapScreen),
		[this](EAsyncPushWidgetState InPushWidgetState, UWidget_ActivatableBase* PushedWidget)
		{
			if (InPushWidgetState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UWidget_KeyRemapScreen* KeyRemapScreen = CastChecked<UWidget_KeyRemapScreen>(PushedWidget);
				KeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &ThisClass::OnKeyToRemapPressed);
				KeyRemapScreen->OnKeyRemapScreenKeySelectCanceled.BindUObject(this, &ThisClass::OnKeyToRemapSelectCanceled);
				
				// 设置键位重映射屏幕需要监听的按键类型（键盘鼠标或手柄）
				if (CachedOwningKeyRemapDataObject)
				{
					KeyRemapScreen->SetDesiredInputTypeToFilter(CachedOwningKeyRemapDataObject->GetDesiredInputType());
				}
				
			}
		}
	);
}

void UWidget_ListEntry_KeyRemap::OnResetKeyBindingButtonClicked()
{
}

void UWidget_ListEntry_KeyRemap::OnKeyToRemapPressed(const FKey& InPressedKey)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CachedOwningKeyRemapDataObject->BindNewInputKey(InPressedKey);
	}
}

void UWidget_ListEntry_KeyRemap::OnKeyToRemapSelectCanceled(const FString& InMsg)
{
	UFrontendUISubsystem::Get(this)->PushModalScreenToModalStackAsync(
		ArcaneGameplayTags::Frontend_Widget_ModalScreen_TimerConfirm,
		EModalType::Ok,
		FText::FromString(TEXT("")),
		FText::FromString(TEXT("")),
		FText::FromString(InMsg),
		FText::FromString(TEXT("")),
		FSlateBrush(),
		[this](EModalButtonType ClickButtonType){},
		EColorThemeType::ErrorTheme,
		FText::FromString(TEXT("退出")) // 按键选择取消的标题文本
	);
}
