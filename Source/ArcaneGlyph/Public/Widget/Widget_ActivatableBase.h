// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonInputTypeEnum.h"
#include "Widget_ActivatableBase.generated.h"

class UWaveProgressBarWidget;
class UCanvasPanel;
class UEnemyUIComponent;
class UHeroUIComponent;
class FCommonUIGameInputProcessor;
class AArcaneHeroController;
/**
 * 
 */

// 定义一个动态多播委托，用于处理输入类型变化事件，对于那些在不同输入类型下需要使用不同UI页面的控件尤为重要
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputTypeChangedDelegate, ECommonInputType, NewInputType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTriggerInputKeyPressedOrReleaseDelegate);	// 定义一个动态多播委托，用于处理触发输入键按下事件

UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnInputTypeChangedDelegate OnInputTypeChanged;	// 输入类型变化事件委托

	UPROPERTY(BlueprintAssignable)
	FOnTriggerInputKeyPressedOrReleaseDelegate OnTriggerInputKeyPressed;	// 触发输入键按下事件委托

	UPROPERTY(BlueprintAssignable)
	FOnTriggerInputKeyPressedOrReleaseDelegate OnTriggerInputKeyReleased;	// 触发输入键释放事件委托

	// 移除武器的MappingContext，以防止与UI交互时发生冲突，如果玩家装备了武器的话
	UFUNCTION(BlueprintCallable)
	void RemoveWeaponMappingContext();
	
	UFUNCTION(BlueprintCallable)
	void AddWeaponMappingContext();
	
protected:
	UFUNCTION(BlueprintPure)
	AArcaneHeroController* GetOwningHeroController();

	void OnRightMouseButtonPressed(const FKey& Key);
	void HandleInputKeyPressed(ECommonInputType InputType);
	void HandleTriggerInputKeyPressed(const FKey& Key, ECommonInputType InputType);
	void HandleTriggerInputKeyReleased(const FKey& Key, ECommonInputType InputType);
	
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	
	TWeakObjectPtr<AArcaneHeroController> CachedOwningPC;	// 缓存拥有的玩家控制器

	TSharedPtr<FCommonUIGameInputProcessor> CachedMouseInputPreprocessor;	// 缓存鼠标输入预处理器，用于处理鼠标输入事件

private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	bool bUseDifferentPageForDifferentInputType = false;	// 是否为不同的输入类型使用不同的页面

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	bool bListenTriggerKey = false;	// 是否监听触发键
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	ECommonInputType CurrentPageInputType = ECommonInputType::MouseAndKeyboard;	// 当前页面输入类型（键盘鼠标或手柄）

	bool bTriggering = false;	// 是否正在触发输入键
	
	UPROPERTY(meta=(BindWidgetOptional))
	UCanvasPanel* WaveRootPanel;
	
	/** 要创建的主进度条UI蓝图类 */
	UPROPERTY(EditDefaultsOnly, Category = "Wave Test")
	TSubclassOf<UWaveProgressBarWidget> WaveProgressBarWidgetClass;
	
	UPROPERTY()
	UWaveProgressBarWidget* WaveProgressBarWidget;
	
	void CreateWaveProgressBarWidget();
	
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
