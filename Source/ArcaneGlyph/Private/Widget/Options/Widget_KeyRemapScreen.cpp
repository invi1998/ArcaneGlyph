// INVI_1998 All Rights Reserved.


#include "Widget/Options/Widget_KeyRemapScreen.h"

#include "ArcaneDebugHelper.h"
#include "CommonInputSubsystem.h"
#include "CommonRichTextBlock.h"
#include "ICommonInputModule.h"
#include  "CommonUITypes.h"
#include "Framework/Application/IInputProcessor.h"

class FKeyRemapScreenInputPreprocessor : public IInputProcessor
{
public:
	explicit FKeyRemapScreenInputPreprocessor(ECommonInputType InInputTypeToListen, ULocalPlayer* InLocalPlayer)
		: CachedInputTypeToListen(InInputTypeToListen), CachedWeakLocalPlayer(InLocalPlayer)
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
		UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(CachedWeakLocalPlayer.Get());
		check(CommonInputSubsystem);

		ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();
			
		switch (CachedInputTypeToListen)
		{
		case ECommonInputType::MouseAndKeyboard:
			if (PressedKey.IsGamepadKey() || CurrentInputType == ECommonInputType::Gamepad)
			{
				// 如果按下的按键是手柄按键，或者当前输入类型是手柄，则取消当前按键选择
				OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT("检测到手柄按键被映射为键盘输入，按键重映射已取消，请使用键盘鼠标输入进行按键重映射"));
				return;
			}
			if (PressedKey == EKeys::Escape || PressedKey == EKeys::RightMouseButton || PressedKey == EKeys::LeftMouseButton)
			{
				OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT("按键重映射已取消 （按下了Escape键或鼠标右键或左键）"));
				return; // 如果按下的是Escape键或者鼠标右键或者左键，则取消当前按键选择
			}
			break;
		case ECommonInputType::Gamepad:
			if (CurrentInputType == ECommonInputType::Gamepad && PressedKey == EKeys::LeftMouseButton)
			{
				// 如果当前输入类型是手柄，并且按下的是手柄确认键（左键），则广播当前按键映射
				/* 那么这里我们应该广播什么键值呢？
				现在我们不能再将这个输入作为键值广播了。
				因为按下游戏手柄上的确认键时，其效果总是等同于鼠标左键点击
				而这并非我们想要的结果
				此外，我们也不应该广播硬编码的键位值，因为游戏手柄上的确认键
				可能根据数据表中设置的值而有所不同 在这里我们应该通过通用 UI 检索映射来确认按键。*/
				FCommonInputActionDataBase* InputActionData = ICommonInputModule::GetSettings().GetDefaultClickAction().GetRow<FCommonInputActionDataBase>(TEXT(""));
				check(InputActionData);
				
				OnInputPreProcessorKeyPressed.ExecuteIfBound(InputActionData->GetDefaultGamepadInputTypeInfo().GetKey());
				return;
			}
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
	TWeakObjectPtr<ULocalPlayer> CachedWeakLocalPlayer; // 缓存本地玩家对象，用于处理输入事件
	
};

void UWidget_KeyRemapScreen::SetDesiredInputTypeToFilter(ECommonInputType InputType)
{
	CachedDesiredInputTypeToListen = InputType;
}

void UWidget_KeyRemapScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	CachedInputPreprocessor = MakeShared<FKeyRemapScreenInputPreprocessor>(CachedDesiredInputTypeToListen, GetOwningLocalPlayer());
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
