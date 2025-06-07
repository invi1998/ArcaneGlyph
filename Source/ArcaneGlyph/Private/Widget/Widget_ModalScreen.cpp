// INVI_1998 All Rights Reserved.


#include "Widget/Widget_ModalScreen.h"

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKScreen(const FText& InTitle, const FText& InMessage, const FText& InDescription, const FSlateBrush& InIcon)
{
	UConfirmScreenInfoObject* NewScreen = NewObject<UConfirmScreenInfoObject>();
	NewScreen->ModalTitle = InTitle;
	NewScreen->ModalMessage = InMessage;
	NewScreen->ModalDescription = InDescription;
	NewScreen->ModalIcon = InIcon;

	FConfirmButtonInfo OKButtonInfo = FConfirmButtonInfo();
	OKButtonInfo.ConfirmButtonType = EModalButtonType::Close;
	OKButtonInfo.ButtonText = FText::FromString(TEXT("确认"));

	NewScreen->AvailableScreenButtons.Add(OKButtonInfo);

	return NewScreen;

}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKCancelScreen(const FText& InTitle, const FText& InMessage, const FText& InDescription, const FSlateBrush& InIcon)
{
	UConfirmScreenInfoObject* NewScreen = NewObject<UConfirmScreenInfoObject>();
	NewScreen->ModalTitle = InTitle;
	NewScreen->ModalMessage = InMessage;
	NewScreen->ModalDescription = InDescription;
	NewScreen->ModalIcon = InIcon;

	FConfirmButtonInfo OkButtonInfo = FConfirmButtonInfo();
	OkButtonInfo.ConfirmButtonType = EModalButtonType::Confirm;
	OkButtonInfo.ButtonText = FText::FromString(TEXT("确认"));

	FConfirmButtonInfo CancelButtonInfo = FConfirmButtonInfo();
	CancelButtonInfo.ConfirmButtonType = EModalButtonType::Cancel;
	CancelButtonInfo.ButtonText = FText::FromString(TEXT("取消"));

	NewScreen->AvailableScreenButtons.Add(OkButtonInfo);
	NewScreen->AvailableScreenButtons.Add(CancelButtonInfo);

	return NewScreen;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InTitle, const FText& InMessage, const FText& InDescription, const FSlateBrush& InIcon)
{
	UConfirmScreenInfoObject* NewScreen = NewObject<UConfirmScreenInfoObject>();
	NewScreen->ModalTitle = InTitle;
	NewScreen->ModalMessage = InMessage;
	NewScreen->ModalDescription = InDescription;
	NewScreen->ModalIcon = InIcon;

	FConfirmButtonInfo YesButtonInfo = FConfirmButtonInfo();
	YesButtonInfo.ConfirmButtonType = EModalButtonType::Confirm;
	YesButtonInfo.ButtonText = FText::FromString(TEXT("确认"));

	FConfirmButtonInfo NoButtonInfo = FConfirmButtonInfo();
	NoButtonInfo.ConfirmButtonType = EModalButtonType::Cancel;
	NoButtonInfo.ButtonText = FText::FromString(TEXT("取消"));

	NewScreen->AvailableScreenButtons.Add(YesButtonInfo);
	NewScreen->AvailableScreenButtons.Add(NoButtonInfo);

	return NewScreen;
}


