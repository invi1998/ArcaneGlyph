// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget_ActivatableBase.h"
#include "ArcaneTypes/ArcaneEnumTypes.h"
#include "Widget_ModalScreen.generated.h"

class UDynamicEntryBox;
class UCommonTextBlock;
class UCommonLazyImage;

USTRUCT(BlueprintType)
struct FConfirmButtonInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Confirm Button")
	EModalButtonType ConfirmButtonType = EModalButtonType::Unknow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Confirm Button")
	FText ButtonText;	// 按钮文本
	
};

UCLASS()
class ARCANEGLYPH_API UConfirmScreenInfoObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	FText ModalTitle;	// 模态框标题文本

	UPROPERTY(Transient)
	FText ModalSubtitle;	// 模态框副标题文本

	UPROPERTY(Transient)
	FText ModalMessage;		// 模态框消息文本

	UPROPERTY(Transient)
	FText ModalDescription;	// 模态框描述文本

	UPROPERTY(Transient)
	FSlateBrush ModalIcon;	// 模态框图标

	UPROPERTY(Transient)
	TArray<FConfirmButtonInfo> AvailableScreenButtons;	// 可变按钮列表

	static UConfirmScreenInfoObject* CreateOKScreen(const FText& InTitle, const FText& InSubTitle, const FText& InMessage, const FText& InDescription = FText::GetEmpty(), const FSlateBrush& InIcon = FSlateBrush());
	static UConfirmScreenInfoObject* CreateOKCancelScreen(const FText& InTitle, const FText& InSubTitle, const FText& InMessage, const FText& InDescription = FText::GetEmpty(), const FSlateBrush& InIcon = FSlateBrush());
	static UConfirmScreenInfoObject* CreateYesNoScreen(const FText& InTitle, const FText& InSubTitle, const FText& InMessage, const FText& InDescription = FText::GetEmpty(), const FSlateBrush& InIcon = FSlateBrush());

};


/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_ModalScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

public:
	// 当这个控件被构建时，在类外部调用该函数，并且在该控件被推入modal栈之前执行
	void InitConfirmScreen(const UConfirmScreenInfoObject* ConfirmScreenInfoObject, TFunction<void(EModalButtonType)> OnButtonClickedCallback, EColorThemeType InColorTheme = EColorThemeType::None);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FSlateColor GetThemeColor() const;	// 获取当前主题颜色

protected:
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

private:
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* Modal_Title;	// 模态框标题文本块（可选绑定）

	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* Modal_Subtitle;	// 模态框副标题文本块（可选绑定）

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* Modal_Message;	// 模态框消息文本块

	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* Modal_Description;	// 模态框描述文本块（可选绑定）

	UPROPERTY(meta=(BindWidgetOptional))
	UCommonLazyImage* Modal_Icon;	// 模态框图标（可选绑定）

	// 这是一个可在运行时动态插入按钮的容器控件
	UPROPERTY(meta=(BindWidget))
	UDynamicEntryBox* DynamicEntryBox_Buttons;	// 模态框按钮动态入口框

	UPROPERTY()
	EColorThemeType ColorTheme = EColorThemeType::None;	// 当前颜色主题类型

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Theme Color", meta = (AllowPrivateAccess = "true"))
	FSlateColor NormalColor;	// 正常主题颜色

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Theme Color", meta = (AllowPrivateAccess = "true"))
	FSlateColor InfoColor;	// 信息主题颜色

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Theme Color", meta = (AllowPrivateAccess = "true"))
	FSlateColor WarningColor;	// 警告主题颜色

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Theme Color", meta = (AllowPrivateAccess = "true"))
	FSlateColor ErrorColor;	// 错误主题颜色
	
	
};
