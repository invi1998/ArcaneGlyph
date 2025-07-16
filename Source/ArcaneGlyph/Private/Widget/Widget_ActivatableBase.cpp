// INVI_1998 All Rights Reserved.


#include "Widget/Widget_ActivatableBase.h"

#include "CommonInputTypeEnum.h"
#include "Controllers/ArcaneHeroController.h"
#include "Framework/Application/IInputProcessor.h"

class FCommonUIMouseInputProcessor : public IInputProcessor
{
public:
	explicit FCommonUIMouseInputProcessor(ULocalPlayer* InLocalPlayer) : CachedWeakLocalPlayer(InLocalPlayer) {}
	
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {}
    
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
		{
			// 模拟键盘后退键按下（根据你的实际绑定键调整）
			FKeyEvent BackEvent(EKeys::BackSpace, FModifierKeysState(), 0, false, 0, 0);
			SlateApp.ProcessKeyDownEvent(BackEvent);
            
			// 标记为已处理，阻止默认右键行为
			return true;
		}
		return false;
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

void UWidget_ActivatableBase::NativeOnActivated()
{
	Super::NativeOnActivated();

}

void UWidget_ActivatableBase::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
}
