// INVI_1998 All Rights Reserved.


#include "AbilitySystem/AbilityTasks/AbilityTask_InputBufferANS.h"

#include "AbilitySystemComponent.h"
#include "ArcaneGameplayTags.h"
#include "EnhancedInputComponent.h"

UAbilityTask_InputBufferANS* UAbilityTask_InputBufferANS::ListenForComboInput(UGameplayAbility* OwningAbility, const TArray<UInputAction*>& ValidActions)
{
	UAbilityTask_InputBufferANS* Task = NewAbilityTask<UAbilityTask_InputBufferANS>(OwningAbility);
	Task->TrackedActions = ValidActions;
	Task->InputBindings.Empty();
	return Task;
}

void UAbilityTask_InputBufferANS::Activate()
{
	if (APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get())
	{
		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			for (UInputAction* Action : TrackedActions)
			{
				// 绑定并存储句柄
				const uint32 Handle = EIC->BindAction(Action, ETriggerEvent::Triggered, this, &UAbilityTask_InputBufferANS::OnInputActionTriggered).GetHandle();
				InputBindings.Add(Action, Handle);
			}
		}
	}
}

void UAbilityTask_InputBufferANS::OnDestroy(bool AbilityEnded)
{
	// 清理输入绑定
	if (APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get())
	{
		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			// 遍历所有存储的句柄进行解绑
			for (const auto& Pair : InputBindings)
			{
				EIC->RemoveBindingByHandle(Pair.Value); // 使用uint32参数
			}
		}
	}
	InputBindings.Empty();
	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_InputBufferANS::OnInputActionTriggered(const FInputActionInstance& ActionInstance)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComboInputDetected.Broadcast(ActionInstance.GetSourceAction());
	}
}
