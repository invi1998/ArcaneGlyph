// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Widget_ActivatableBase.generated.h"

class UEnemyUIComponent;
class UHeroUIComponent;
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

UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_ActivatableArcane : public UWidget_ActivatableBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitEnemyCreatedWidget(AActor* InEnemyActor);

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Arcane | Widget", meta = (DisplayName = "On Owning Hero UI Component Initialized"))
	void BP_OnOwningHeroUIComponentInitialized(UHeroUIComponent* InHeroUIComponent);

	UFUNCTION(BlueprintImplementableEvent, Category = "Arcane | Widget", meta = (DisplayName = "On Enemy Created Widget Initialized"))
	void BP_OnEnemyCreatedWidgetInitialized(UEnemyUIComponent* InEnemyUIComponent);
	
	
};
