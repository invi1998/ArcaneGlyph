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

protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
		
	}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		Debug::Print(TEXT("HandleKeyDownEvent") + InKeyEvent.GetKey().GetDisplayName().ToString());

		return true;
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		Debug::Print(TEXT("HandleMouseButtonDownEvent") + MouseEvent.GetEffectingButton().GetDisplayName().ToString());

		return true;
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
