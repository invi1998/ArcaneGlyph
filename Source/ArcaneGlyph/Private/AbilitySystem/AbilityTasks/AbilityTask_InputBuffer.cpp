// INVI_1998 All Rights Reserved.


#include "AbilitySystem/AbilityTasks/AbilityTask_InputBuffer.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"

UAbilityTask_InputBuffer* UAbilityTask_InputBuffer::CreateInputBufferTask(UGameplayAbility* OwningAbility, UInputAction* InputAction, float BufferDuration)
{
	// 创建一个新的任务实例
	UAbilityTask_InputBuffer* InputBufferTask = NewAbilityTask<UAbilityTask_InputBuffer>(OwningAbility);
	if (!InputBufferTask || !InputAction)
	{
		return nullptr;
	}

	// 设置任务的输入动作和缓冲时间
	InputBufferTask->TargetInputAction = InputAction;
	InputBufferTask->BufferTimeWindow = BufferDuration;
	return InputBufferTask;
}

void UAbilityTask_InputBuffer::Activate()
{
	Super::Activate();

	if (!AbilitySystemComponent.IsValid() || !TargetInputAction)
	{
		return;
	}

	// 获取EnhancedInput组件
	if (APawn* AvatarPawn = Cast<APawn>(AbilitySystemComponent->GetAvatarActor()))
	{
		if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(AvatarPawn->InputComponent))
		{
			// 绑定输入事件
			InputComponent->BindAction(TargetInputAction, ETriggerEvent::Triggered, this, &UAbilityTask_InputBuffer::OnInputTriggered);
		}
	}
}

void UAbilityTask_InputBuffer::OnInputTriggered(const FInputActionInstance& Instance)
{
	// 重置缓冲计时器
	if (BufferTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(BufferTimerHandle);
	}

	// 记录输入并启动缓冲窗口
	bHasBufferedInput = true;
	GetWorld()->GetTimerManager().SetTimer(
		BufferTimerHandle,
		this,
		&UAbilityTask_InputBuffer::BroadcastBufferedAction,
		BufferTimeWindow,
		false);
}

void UAbilityTask_InputBuffer::BroadcastBufferedAction()
{
	if (bHasBufferedInput)
	{
		// 广播有效输入
		OnBufferedInput.Broadcast(TargetInputAction);
		bHasBufferedInput = false;
	}

	// 结束任务
	EndTask();
}
