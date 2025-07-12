// INVI_1998 All Rights Reserved.


#include "Widget/Widget_ModalScreen.h"

#include <CommonButtonBase.h>
#include "CommonTextBlock.h"
#include "CommonLazyImage.h"
#include "Components/DynamicEntryBox.h"
#include "Widget/Component/FrontendCommonButtonBase.h"
#include "ICommonInputModule.h"

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKScreen(const FText& InTitle, const FText& InSubTitle, const FText& InMessage, const FText& InDescription, const FSlateBrush& InIcon, const FText& InOkButtonText)
{
	UConfirmScreenInfoObject* NewScreen = NewObject<UConfirmScreenInfoObject>();
	NewScreen->ModalTitle = InTitle;
	NewScreen->ModalSubtitle = InSubTitle;
	NewScreen->ModalMessage = InMessage;
	NewScreen->ModalDescription = InDescription;
	NewScreen->ModalIcon = InIcon;

	FConfirmButtonInfo OKButtonInfo = FConfirmButtonInfo();
	OKButtonInfo.ConfirmButtonType = EModalButtonType::Close;
	OKButtonInfo.ButtonText = InOkButtonText.IsEmpty() ? FText::FromString(TEXT("确认")) : InOkButtonText;

	NewScreen->AvailableScreenButtons.Add(OKButtonInfo);

	return NewScreen;

}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKCancelScreen(const FText& InTitle, const FText& InSubTitle, const FText& InMessage, const FText& InDescription, const FSlateBrush& InIcon, const FText& InOkButtonText, const FText& InCancelButtonText)
{
	UConfirmScreenInfoObject* NewScreen = NewObject<UConfirmScreenInfoObject>();
	NewScreen->ModalTitle = InTitle;
	NewScreen->ModalSubtitle = InSubTitle;
	NewScreen->ModalMessage = InMessage;
	NewScreen->ModalDescription = InDescription;
	NewScreen->ModalIcon = InIcon;

	FConfirmButtonInfo OkButtonInfo = FConfirmButtonInfo();
	OkButtonInfo.ConfirmButtonType = EModalButtonType::Confirm;
	OkButtonInfo.ButtonText = InOkButtonText.IsEmpty() ? FText::FromString(TEXT("确认")) : InOkButtonText;

	FConfirmButtonInfo CancelButtonInfo = FConfirmButtonInfo();
	CancelButtonInfo.ConfirmButtonType = EModalButtonType::Cancel;
	CancelButtonInfo.ButtonText = InCancelButtonText.IsEmpty() ? FText::FromString(TEXT("取消")) : InCancelButtonText;

	NewScreen->AvailableScreenButtons.Add(OkButtonInfo);
	NewScreen->AvailableScreenButtons.Add(CancelButtonInfo);

	return NewScreen;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InTitle, const FText& InSubTitle, const FText& InMessage, const FText& InDescription, const FSlateBrush& InIcon, const FText& InYesButtonText, const FText& InNoButtonText)
{
	UConfirmScreenInfoObject* NewScreen = NewObject<UConfirmScreenInfoObject>();
	NewScreen->ModalTitle = InTitle;
	NewScreen->ModalSubtitle = InSubTitle;
	NewScreen->ModalMessage = InMessage;
	NewScreen->ModalDescription = InDescription;
	NewScreen->ModalIcon = InIcon;

	FConfirmButtonInfo YesButtonInfo = FConfirmButtonInfo();
	YesButtonInfo.ConfirmButtonType = EModalButtonType::Confirm;
	YesButtonInfo.ButtonText = InYesButtonText.IsEmpty() ? FText::FromString(TEXT("确认")) : InYesButtonText;

	FConfirmButtonInfo NoButtonInfo = FConfirmButtonInfo();
	NoButtonInfo.ConfirmButtonType = EModalButtonType::Cancel;
	NoButtonInfo.ButtonText = InNoButtonText.IsEmpty() ? FText::FromString(TEXT("取消")) : InNoButtonText;

	NewScreen->AvailableScreenButtons.Add(YesButtonInfo);
	NewScreen->AvailableScreenButtons.Add(NoButtonInfo);

	return NewScreen;
}

void UWidget_ModalScreen::InitConfirmScreen(const UConfirmScreenInfoObject* ConfirmScreenInfoObject, TFunction<void(EModalButtonType)> OnButtonClickedCallback, EColorThemeType InColorTheme)
{
	check(Modal_Message);
	ColorTheme = InColorTheme;
	if (ConfirmScreenInfoObject)
	{
		if (Modal_Title)
		{
			Modal_Title->SetText(ConfirmScreenInfoObject->ModalTitle);
		}
		if (Modal_Subtitle)
		{
			Modal_Subtitle->SetText(ConfirmScreenInfoObject->ModalSubtitle);
		}
		if (Modal_Message)
		{
			Modal_Message->SetText(ConfirmScreenInfoObject->ModalMessage);
		}
		if (Modal_Icon)
		{
			Modal_Icon->SetBrush(ConfirmScreenInfoObject->ModalIcon);
		}
		if (DynamicEntryBox_Buttons->GetNumEntries() > 0)
		{
			/*
			 * 清除动态入口框中的所有按钮，清除的控件类型是子类蓝图里特殊创建的（UFrontendCommonButtonBase)
			 * 然后我们将传递一个清理函数，该函数将按钮的点击事件进行清理，确保我们的代理整洁
			 */
			DynamicEntryBox_Buttons->Reset<UFrontendCommonButtonBase>(
				[](const UFrontendCommonButtonBase& Button)
				{
					Button.OnClicked().Clear();
				}
			);
			
		}
		
		for (const FConfirmButtonInfo& ButtonInfo : ConfirmScreenInfoObject->AvailableScreenButtons)
		{
			FDataTableRowHandle InputActionRowHandle;
			switch (ButtonInfo.ConfirmButtonType)
			{
			case EModalButtonType::Confirm:
				// 如果是确认按钮，则使用默认的点击输入动作
				InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultClickAction();
				break;
			case EModalButtonType::Cancel:
				// 如果是取消按钮，则使用默认的取消输入动作
				InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
				break;
			case EModalButtonType::Close:
				// 如果是关闭按钮，则使用默认的取消输入动作
				InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
				break;
			case EModalButtonType::Unknow:
				break;
			}
			
			UFrontendCommonButtonBase* AddButton = DynamicEntryBox_Buttons->CreateEntry<UFrontendCommonButtonBase>();
			// 设置按钮文本
			AddButton->SetButtenText(ButtonInfo.ButtonText);
			// 设置按钮的输入动作
			// AddButton->SetTriggeredInputAction(InputActionRowHandle);
			AddButton->SetTriggeringInputAction(InputActionRowHandle);
			// 绑定按钮点击事件
			AddButton->OnClicked().AddLambda(
				[OnButtonClickedCallback, ButtonInfo, this]()
				{
					OnButtonClickedCallback(ButtonInfo.ConfirmButtonType);
					// 在完成点击事件绑定后，我们需要手动停用该控件
					DeactivateWidget();
				}
			);
			// 设置按钮的主题颜色
			AddButton->SetButtonTheme(InColorTheme);
		}

		if (DynamicEntryBox_Buttons->GetNumEntries() > 0)
		{
			// 如果动态入口框中有按钮，就将最后一个按钮设置为焦点
			DynamicEntryBox_Buttons->GetAllEntries().Last()->SetFocus();
		}
		
	}
}

FSlateColor UWidget_ModalScreen::GetThemeColor() const
{
	switch (ColorTheme) {
	case EColorThemeType::None:
		return FLinearColor::Transparent;
	case EColorThemeType::ClearTheme:
		return FLinearColor::Transparent;
	case EColorThemeType::NormalTheme:
		return NormalColor;
	case EColorThemeType::InfoTheme:
		return InfoColor;
	case EColorThemeType::WarningTheme:
		return WarningColor;
	case EColorThemeType::ErrorTheme:
		return ErrorColor;
	}
	return FLinearColor::Transparent;
}

UWidget* UWidget_ModalScreen::NativeGetDesiredFocusTarget() const
{
	if (DynamicEntryBox_Buttons && DynamicEntryBox_Buttons->GetNumEntries() > 0)
	{
		// 如果动态入口框中有按钮，就将最后一个按钮设置为焦点
		return DynamicEntryBox_Buttons->GetAllEntries().Last();
	}
	
	return Super::NativeGetDesiredFocusTarget();
}


