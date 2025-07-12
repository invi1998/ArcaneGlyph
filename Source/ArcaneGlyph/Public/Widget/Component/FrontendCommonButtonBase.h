// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "ArcaneTypes/ArcaneEnumTypes.h"
#include "FrontendCommonButtonBase.generated.h"

class UCommonLazyImage;
class UCommonTextBlock;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UFrontendCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtenText(FText InButtonText);

	UFUNCTION(BlueprintCallable)
	FText GetButtonDisplayText() const;

	UFUNCTION(BlueprintCallable)
	void SetButtonDisplayIcon(const FSlateBrush& InButtonIcon);

	void SetButtonTheme(EColorThemeType InColorTheme);

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;

private:
	//*** Bound Widgets (绑定的控件) ***//
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_ButtonText;	// 按钮文本块（可选绑定）

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonLazyImage* CommonLazyImage_ButtonIcon;	// 按钮图标（可选绑定）
	// ** Bound Widgets (绑定的控件) ***//

	// 按钮文本
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDisplayText;

	// 是否将按钮文本转换为大写
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	bool bUserUpperCaseButtonText = false;

	// 按钮描述文本
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Button Theme", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCommonButtonStyle> ClearThemeButtonStyle;	// 清除主题按钮样式

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Button Theme", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCommonButtonStyle> NormalThemeButtonStyle;	// 普通主题按钮样式

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Button Theme", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCommonButtonStyle> InfoThemeButtonStyle;	// 信息主题按钮样式

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Button Theme", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCommonButtonStyle> WarningThemeButtonStyle;	// 警告主题按钮样式

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Button Theme", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCommonButtonStyle> ErrorThemeButtonStyle;	// 错误主题按钮样式
	
};
