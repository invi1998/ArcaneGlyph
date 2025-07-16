// INVI_1998 All Rights Reserved.


#include "Widget/Widget_ActivatableBase.h"

#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "Controllers/ArcaneHeroController.h"
#include "Framework/Application/IInputProcessor.h"

class FCommonUIMouseInputProcessor : public IInputProcessor
{
public:
	explicit FCommonUIMouseInputProcessor(ULocalPlayer* InLocalPlayer) : CachedWeakLocalPlayer(InLocalPlayer) {}

	DECLARE_DELEGATE_OneParam(FOnMouseInputPreProcessorKeyPressedDelegate, const FKey& /* PressedKey */);
	FOnMouseInputPreProcessorKeyPressedDelegate OnRightMouseButtonPressed;

protected:
	
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {}
    
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		ProcessPressedKey(MouseEvent.GetEffectingButton());
		return false;
	}

	void ProcessPressedKey(const FKey& PressedKey) const
	{
		UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(CachedWeakLocalPlayer.Get());
		check(CommonInputSubsystem);

		ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();

		if (CurrentInputType == ECommonInputType::MouseAndKeyboard && PressedKey == EKeys::RightMouseButton)
		{
			OnRightMouseButtonPressed.ExecuteIfBound(PressedKey);
		}
		
	}


private:
	TWeakObjectPtr<ULocalPlayer> CachedWeakLocalPlayer; // 缓存本地玩家对象，用于处理输入事件
};


AArcaneHeroController* UWidget_ActivatableBase::GetOwningHeroController()
{
	if (!CachedOwningPC.IsValid())
	{
		CachedOwningPC = GetOwningPlayer<AArcaneHeroController>();
	}

	return CachedOwningPC.IsValid() ? CachedOwningPC.Get() : nullptr;
}

void UWidget_ActivatableBase::OnRightMouseButtonPressed(const FKey& Key)
{
	DeactivateWidget();
}

void UWidget_ActivatableBase::NativeOnActivated()
{
	Super::NativeOnActivated();

	// 如果当前页面是可回退页面，则注册鼠标输入处理器
	if (bIsBackHandler)
	{
		CachedMouseInputPreprocessor = MakeShared<FCommonUIMouseInputProcessor>(GetOwningLocalPlayer());
		CachedMouseInputPreprocessor->OnRightMouseButtonPressed.BindUObject(this, &ThisClass::OnRightMouseButtonPressed);

		FSlateApplication::Get().RegisterInputPreProcessor(CachedMouseInputPreprocessor, -1);
	}

}

void UWidget_ActivatableBase::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (CachedMouseInputPreprocessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(CachedMouseInputPreprocessor);	//	注销鼠标输入处理器
		CachedMouseInputPreprocessor.Reset();	// 重置鼠标输入处理器
	}
}
