// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widget_PrimaryLayout.generated.h"

class UCommonActivatableWidgetContainerBase;

/**
 * 标记为抽象类的主布局小部件，同时禁用原生Tick。
 * 抽象类意味着这个类不能被实例化，必须由子类继承并实现其功能。
 * 同时因为我们不需要加载所有控件，所以禁用原生Tick可以提高性能。
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_PrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(const FGameplayTag& WidgetTag) const;

protected:
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetToStack(UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag WidgetTag, UCommonActivatableWidgetContainerBase* WidgetContainer);

private:
	// Transient 瞬态属性，表示该属性不会被序列化或保存到磁盘，加载时总是会被初始化为0，这就使得它很适合缓存临时运行的值
	UPROPERTY(Transient)
	TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> RegisteredWidgetsStackMap;
	
};
