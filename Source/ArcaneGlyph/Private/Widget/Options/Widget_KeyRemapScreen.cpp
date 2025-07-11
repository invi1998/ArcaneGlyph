// INVI_1998 All Rights Reserved.


#include "Widget/Options/Widget_KeyRemapScreen.h"

#include "ArcaneDebugHelper.h"
#include "Framework/Application/IInputProcessor.h"

class FKeyRemapScreenInputPreprocessor : public IInputProcessor
{
public:
	explicit FKeyRemapScreenInputPreprocessor(ECommonInputType InInputTypeToListen = ECommonInputType::MouseAndKeyboard)
		: CachedInputTypeToListen(InInputTypeToListen)
	{
	}

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeyPressedDelegate, const FKey& /* PressedKey */);
	FOnInputPreProcessorKeyPressedDelegate OnInputPreProcessorKeyPressed;

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeySelectCanceledDelegate, const FString& /* CanceledReason */);
	FOnInputPreProcessorKeySelectCanceledDelegate OnInputPreProcessorKeySelectCanceled;
	
protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
		
	}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		ProcessPressedKey(InKeyEvent.GetKey()); // 处理按键按下事件

		return true;
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		ProcessPressedKey(MouseEvent.GetEffectingButton()); // 处理鼠标按键按下事件
		return true;
	}

	void ProcessPressedKey(const FKey& PressedKey) const
	{
		if (PressedKey == EKeys::Escape || PressedKey == EKeys::RightMouseButton)
		{
			OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT("按键重映射已取消"));
			return; // 如果按下的是Escape键或者鼠标右键或者左键，则取消当前按键选择
		}

		switch (CachedInputTypeToListen)
		{
		case ECommonInputType::MouseAndKeyboard:
			if (PressedKey.IsGamepadKey())
			{
				OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT("检测到手柄按键被映射为键盘输入，按键重映射已取消，请使用键盘鼠标输入进行按键重映射"));
				return;
			}
			break;
		case ECommonInputType::Gamepad:
			if (!PressedKey.IsGamepadKey())
			{
				OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT("检测到键盘鼠标按键被映射为手柄输入，按键重映射已取消，请使用手柄输入进行按键重映射"));
				return; // 如果按下的不是手柄按键，则取消当前按键选择
			}
			break;
		default:
			break;
		}

		OnInputPreProcessorKeyPressed.ExecuteIfBound(PressedKey);
		
	}

private:
	ECommonInputType CachedInputTypeToListen; // 缓存当前输入类型，用于判断是否需要处理键盘鼠标或手柄输入
};

void UWidget_KeyRemapScreen::SetDesiredInputTypeToFilter(ECommonInputType InputType)
{
	CachedDesiredInputTypeToListen = InputType;
}

void UWidget_KeyRemapScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	CachedInputPreprocessor = MakeShared<FKeyRemapScreenInputPreprocessor>();
	if (FSlateApplication::IsInitialized())
	{
		// -1 优先级表示该输入预处理器处于最高优先级，它需要拦截所有输入
		FSlateApplication::Get().RegisterInputPreProcessor(CachedInputPreprocessor, -1); // 注册输入预处理器，优先级为-1
	}
}

void UWidget_KeyRemapScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (FSlateApplication::IsInitialized() && CachedInputPreprocessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(CachedInputPreprocessor); // 注销输入预处理器
		CachedInputPreprocessor.Reset(); // 重置输入预处理器(释放内存)
	}
}
