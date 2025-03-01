// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_ExecuteTaskOnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTaskDelegate, float, DeltaTime);

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UAbilityTask_ExecuteTaskOnTick : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTask_ExecuteTaskOnTick();
	
	// HidePin：隐藏指定的引脚，DefaultToSelf：默认自己，在蓝图中调用时，无需手动连接 `OwningAbility` 引脚，自动绑定当前 Ability。，BlueprintInternalUseOnly：蓝图内部使用，只能在事件图中使用
	UFUNCTION(BlueprintCallable, Category = "Arcane | AbilityTasks", meta = (HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAbilityTask_ExecuteTaskOnTick* ExecuteTaskOnTick(UGameplayAbility* OwningAbility);

	// UAbilityTask interface
	virtual void TickTask(float DeltaTime) override;
	// ~UAbilityTask interface
	
	UPROPERTY(BlueprintAssignable, Category = "Arcane | AbilityTasks")
	FOnAbilityTaskDelegate OnAbilityTaskTick;		// 每帧执行的委托
	
};
