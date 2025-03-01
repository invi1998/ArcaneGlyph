// INVI_1998 All Rights Reserved.


#include "AbilitySystem/AbilityTasks/AbilityTask_ExecuteTaskOnTick.h"

UAbilityTask_ExecuteTaskOnTick::UAbilityTask_ExecuteTaskOnTick()
{
	bTickingTask = true;		// 设置为true，表示这是一个需要每帧执行的任务
}

UAbilityTask_ExecuteTaskOnTick* UAbilityTask_ExecuteTaskOnTick::ExecuteTaskOnTick(UGameplayAbility* OwningAbility)
{
	UAbilityTask_ExecuteTaskOnTick* TaskOnTickNode = NewAbilityTask<UAbilityTask_ExecuteTaskOnTick>(OwningAbility);

	return TaskOnTickNode;
}

void UAbilityTask_ExecuteTaskOnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	// - **作用**：检查任务是否仍处于活跃状态（未被手动结束或外部终止）。
	// **必要性**：防止在任务结束后仍广播委托，导致访问无效内存或逻辑错误。
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnAbilityTaskTick.Broadcast(DeltaTime);
	}
	else
	{
		EndTask();
	}
}
