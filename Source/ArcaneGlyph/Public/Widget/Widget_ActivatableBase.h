// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Widget_ActivatableBase.generated.h"

class FCommonUIMouseInputProcessor;
class AArcaneHeroController;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure)
	AArcaneHeroController* GetOwningHeroController();

	void OnRightMouseButtonPressed(const FKey& Key);
	
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	

private:
	TWeakObjectPtr<AArcaneHeroController> CachedOwningPC;	// 缓存拥有的玩家控制器

	TSharedPtr<FCommonUIMouseInputProcessor> CachedMouseInputPreprocessor;	// 缓存鼠标输入预处理器，用于处理鼠标输入事件

	
};
