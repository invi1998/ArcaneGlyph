// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget/Widget_ActivatableBase.h"
#include "CommonInputTypeEnum.h"
#include "Widget_KeyRemapScreen.generated.h"

class UCommonRichTextBlock;
class FKeyRemapScreenInputPreprocessor;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_KeyRemapScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

public:
	void SetDesiredInputTypeToFilter(ECommonInputType InputType);

	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeyPressedDelegate, const FKey& InPressedKey);	// 键位重映射按键按下的委托
	FOnKeyRemapScreenKeyPressedDelegate OnKeyRemapScreenKeyPressed;	// 键位重映射按键按下的委托，用于处理按键重映射的输入事件

	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeySelectCanceledDelegate, const FString& InMsg);	// 键位重映射按键选择取消的委托
	FOnKeyRemapScreenKeySelectCanceledDelegate OnKeyRemapScreenKeySelectCanceled;	// 键位重映射按键选择取消的委托，用于处理按键重映射的输入事件
	
protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	

private:
	void OnValidKeyPressedDelegate(const FKey& InPressedKey);	// 有效按键按下的委托，用于处理按键重映射的输入事件
	void OnInvalidKeyPressedDelegate(const FString& InMsg);	// 无效按键按下的委托，用于处理按键重映射的输入事件

	// 延迟Tick确保输入按键在控件停用之前和回调函数调用之前被正确捕获
	void RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback);	// 请求停用小部件，执行预停用回调函数
	
	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_KeyRemapMessage;	// 键位重映射提示文本块

	TSharedPtr<FKeyRemapScreenInputPreprocessor> CachedInputPreprocessor;	// 键位重映射输入预处理器，用于处理键位重映射的输入事件

	ECommonInputType CachedDesiredInputTypeToListen;	// 缓存的期望输入类型，用于监听键位重映射的输入事件（键盘鼠标或手柄）

};
