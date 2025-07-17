// INVI_1998 All Rights Reserved.


#include "Widget/Widget_ActivatableBase.h"

#include "CommonInputSubsystem.h"
#include "Controllers/ArcaneHeroController.h"
#include "Framework/Application/IInputProcessor.h"
#include "Interfaces/PawnUIInterface.h"

class FCommonUIGameInputProcessor : public IInputProcessor
{
public:
	explicit FCommonUIGameInputProcessor(ULocalPlayer* InLocalPlayer) : CachedWeakLocalPlayer(InLocalPlayer) {}

	DECLARE_DELEGATE_OneParam(FOnMouseInputPreProcessorKeyPressedDelegate, const FKey& /* PressedKey */);
	FOnMouseInputPreProcessorKeyPressedDelegate OnRightMouseButtonPressed;

	DECLARE_DELEGATE_TwoParams(FOnInputKeyPressedOrReleaseDelegate, const FKey& , ECommonInputType );
	FOnInputKeyPressedOrReleaseDelegate OnAnyKeyPressed;	// 任何按键按下的委托

	FOnInputKeyPressedOrReleaseDelegate OnTriggerKeyPressed;	// 触发按键按下的委托
	FOnInputKeyPressedOrReleaseDelegate OnTriggerKeyReleased;	// 触发按键释放的委托

protected:
	
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {}
    
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		ProcessPressedKey(MouseEvent.GetEffectingButton());
		return false;
	}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		ProcessPressedKey(InKeyEvent.GetKey());
		return false;
	}

	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		// 处理按键释放事件
		ECommonInputType CurrentInputType = UCommonInputSubsystem::Get(CachedWeakLocalPlayer.Get())->GetCurrentInputType();

		// 如果当前输入类型是游戏手柄，并且按键是右扳机键
		if (CurrentInputType == ECommonInputType::Gamepad && InKeyEvent.GetKey() == EKeys::Gamepad_RightTrigger)
		{
			OnTriggerKeyReleased.ExecuteIfBound(InKeyEvent.GetKey(), CurrentInputType); // 执行触发按键释放的委托
		}
		return false;
	}

	void ProcessPressedKey(const FKey& PressedKey) const
	{
		UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(CachedWeakLocalPlayer.Get());
		check(CommonInputSubsystem);

		ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();

		OnAnyKeyPressed.ExecuteIfBound(PressedKey, CurrentInputType);	// 执行任何按键按下的委托

		if (CurrentInputType == ECommonInputType::Gamepad && PressedKey == EKeys::Gamepad_RightTrigger)
		{
			OnTriggerKeyPressed.ExecuteIfBound(PressedKey, CurrentInputType); // 执行触发按键按下的委托
		}
		
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

void UWidget_ActivatableBase::HandleInputKeyPressed(const FKey& Key, ECommonInputType InputType)
{
	if (bUseDifferentPageForDifferentInputType)
	{
		if (CurrentPageInputType != InputType)
		{
			OnInputTypeChanged.Broadcast(InputType);	// 如果当前输入类型与页面输入类型不一致，则触发输入类型改变事件
		}
	}
}

void UWidget_ActivatableBase::HandleTriggerInputKeyPressed(const FKey& Key, ECommonInputType InputType)
{
	if (bListenTriggerKey && !bTriggering)
	{
		if (CurrentPageInputType == InputType)
		{
			OnTriggerInputKeyPressed.Broadcast();
			bTriggering = true;	// 设置触发状态为 true，表示正在触发
		}
	}
}

void UWidget_ActivatableBase::HandleTriggerInputKeyReleased(const FKey& Key, ECommonInputType InputType)
{
	if (bListenTriggerKey && bTriggering)
	{
		if (CurrentPageInputType == InputType)
		{
			OnTriggerInputKeyReleased.Broadcast();
			bTriggering = false;	// 设置触发状态为 false，表示触发结束
		}
	}
}

void UWidget_ActivatableBase::NativeOnActivated()
{
	Super::NativeOnActivated();

	// 如果当前页面是可回退页面，则注册鼠标输入处理器
	if (bIsBackHandler)
	{
		CachedMouseInputPreprocessor = MakeShared<FCommonUIGameInputProcessor>(GetOwningLocalPlayer());
		CachedMouseInputPreprocessor->OnRightMouseButtonPressed.BindUObject(this, &ThisClass::OnRightMouseButtonPressed);
		
	}

	if (bUseDifferentPageForDifferentInputType)
	{
		if (CachedMouseInputPreprocessor)
		{
			CachedMouseInputPreprocessor->OnAnyKeyPressed.BindUObject(this, &ThisClass::HandleInputKeyPressed);
		}
		else
		{
			CachedMouseInputPreprocessor = MakeShared<FCommonUIGameInputProcessor>(GetOwningLocalPlayer());
			CachedMouseInputPreprocessor->OnAnyKeyPressed.BindUObject(this, &ThisClass::HandleInputKeyPressed);
		}
	}

	if (bListenTriggerKey)
	{
		if (CachedMouseInputPreprocessor)
		{
			CachedMouseInputPreprocessor->OnTriggerKeyPressed.BindUObject(this, &ThisClass::HandleTriggerInputKeyPressed);
			CachedMouseInputPreprocessor->OnTriggerKeyReleased.BindUObject(this, &ThisClass::HandleTriggerInputKeyReleased);
		}
		else
		{
			CachedMouseInputPreprocessor = MakeShared<FCommonUIGameInputProcessor>(GetOwningLocalPlayer());
			CachedMouseInputPreprocessor->OnTriggerKeyPressed.BindUObject(this, &ThisClass::HandleTriggerInputKeyPressed);
			CachedMouseInputPreprocessor->OnTriggerKeyReleased.BindUObject(this, &ThisClass::HandleTriggerInputKeyReleased);
		}
	}

	if (CachedMouseInputPreprocessor)
	{
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

void UWidget_ActivatableArcane::InitEnemyCreatedWidget(AActor* InEnemyActor)
{
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(InEnemyActor))
	{
		if (UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent())
		{
			BP_OnEnemyCreatedWidgetInitialized(EnemyUIComponent);
		}
	}
}

void UWidget_ActivatableArcane::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UHeroUIComponent* HeroUIComponent = PawnUIInterface->GetHeroUIComponent())
		{
			BP_OnOwningHeroUIComponentInitialized(HeroUIComponent);
		}
	}
}
