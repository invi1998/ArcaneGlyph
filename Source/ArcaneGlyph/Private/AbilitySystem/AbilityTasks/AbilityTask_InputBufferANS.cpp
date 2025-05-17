// INVI_1998 All Rights Reserved.


#include "AbilitySystem/AbilityTasks/AbilityTask_InputBufferANS.h"

#include "AbilitySystemComponent.h"
#include "ArcaneGameplayTags.h"
#include "EnhancedInputComponent.h"


UAbilityTask_InputBufferANS* UAbilityTask_InputBufferANS::CreateInputBufferTaskANS(UGameplayAbility* OwningAbility, FGameplayTag WindowTag)
{
	UAbilityTask_InputBufferANS* Task = NewAbilityTask<UAbilityTask_InputBufferANS>(OwningAbility);
	Task->AssociatedWindowTag = WindowTag;
	return Task;
}

void UAbilityTask_InputBufferANS::Activate()
{
	Super::Activate();

	// 绑定ANS窗口事件
	if (AbilitySystemComponent.IsValid())
	{
		// 监听窗口开启事件
		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(ArcaneGameplayTags::Player_Event_InputBuffer_WindowOpened).AddUObject(this, &UAbilityTask_InputBufferANS::OnInputWindowOpened);

		// 监听窗口关闭事件
		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(ArcaneGameplayTags::Player_Event_InputBuffer_WindowClosed).AddUObject(this, &UAbilityTask_InputBufferANS::OnInputWindowClosed);

	}

	// 绑定输入事件（EnhancedInput)
	if (APawn* Pawn = Cast<APawn>(GetAvatarActor()))
	{
		if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(Pawn->InputComponent))
		{
			// 遍历所有需要跟踪的输入动作
			for (const UInputAction* Action : TrackedInputActions)
			{
				InputComponent->BindAction(Action, ETriggerEvent::Triggered, this, &UAbilityTask_InputBufferANS::OnInputActionTriggered);
			}
		}
	}
}

void UAbilityTask_InputBufferANS::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_InputBufferANS::OnInputActionTriggered(const FInputActionInstance& Instance)
{
	// 仅在输入缓冲窗口激活时记录
	if (AbilitySystemComponent.IsValid() && AbilitySystemComponent->HasMatchingGameplayTag(ArcaneGameplayTags::Player_Ability_InputBuffer_Active))
	{
		BufferedActions.AddUnique(Instance.GetSourceAction());
	}
}

void UAbilityTask_InputBufferANS::OnInputWindowOpened(const FGameplayEventData* EventData)
{
	// 清空旧的缓冲输入
	BufferedActions.Empty();
}

void UAbilityTask_InputBufferANS::OnInputWindowClosed(const FGameplayEventData* EventData)
{
	if (BufferedActions.Num() > 0)
	{
		OnBufferedInput.Broadcast(BufferedActions.Last());
		BufferedActions.Empty();
	}

	EndTask();
}
