// INVI_1998 All Rights Reserved.


#include "Widget/Options/Widget_KeyRemapScreen.h"

#include "ArcaneDebugHelper.h"
#include "CommonRichTextBlock.h"
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
	CachedInputPreprocessor->OnInputPreProcessorKeyPressed.BindUObject(this, &ThisClass::OnValidKeyPressedDelegate);
	CachedInputPreprocessor->OnInputPreProcessorKeySelectCanceled.BindUObject(this, &ThisClass::OnInvalidKeyPressedDelegate);

	if (FSlateApplication::IsInitialized())
	{
		// -1 优先级表示该输入预处理器处于最高优先级，它需要拦截所有输入
		FSlateApplication::Get().RegisterInputPreProcessor(CachedInputPreprocessor, -1); // 注册输入预处理器，优先级为-1

		FString InputDeviceName;
		switch (CachedDesiredInputTypeToListen)
		{
		case ECommonInputType::MouseAndKeyboard:
			InputDeviceName = TEXT("键盘 或者 鼠标");
			break;
		case ECommonInputType::Gamepad:
			InputDeviceName = TEXT("游戏手柄");
			break;
		default:
			InputDeviceName = TEXT("未知输入设备");
			break;
		}

		const FString InfoMsg = FString::Printf(
			TEXT("<KeyRemapDefault>按下任意</><KeyRemapHighlight> %s </><KeyRemapDefault>按键 用以替换当前输入按键</>"), *InputDeviceName);

		CommonRichTextBlock_KeyRemapMessage->SetText(FText::FromString(InfoMsg));

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

void UWidget_KeyRemapScreen::OnValidKeyPressedDelegate(const FKey& InPressedKey)
{
	RequestDeactivateWidget(
		[this, InPressedKey]()
		{
			OnKeyRemapScreenKeyPressed.ExecuteIfBound(InPressedKey);
		}
	); // 请求关闭当前按键重映射界面
}

void UWidget_KeyRemapScreen::OnInvalidKeyPressedDelegate(const FString& InMsg)
{
	RequestDeactivateWidget(
		[this, InMsg]()
		{
			OnKeyRemapScreenKeySelectCanceled.ExecuteIfBound(InMsg);
		}
	);
}

void UWidget_KeyRemapScreen::RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback)
{
	// 延迟Tick确保输入按键被处理完毕后再执行回调
	FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateLambda(
			[PreDeactivateCallback, this](float DeltaTime)->bool
			{
				PreDeactivateCallback();
				DeactivateWidget();		// 调用父类的 DeactivateWidget 函数来关闭当前按键重映射界面

				// 如果返回 true，这个回调函数将继续保持计时状态
				return false; // 返回 false 以确保该 ticker 只执行一次
			}
		)
	);
}
