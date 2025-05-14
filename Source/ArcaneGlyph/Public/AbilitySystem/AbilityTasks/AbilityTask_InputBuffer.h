// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_InputBuffer.generated.h"

class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputBufferDelegate, const UInputAction*, BufferedAction);

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UAbilityTask_InputBuffer : public UAbilityTask
{
	GENERATED_BODY()

public:
	// 创建任务实例的静态方法
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Input Buffer", BlueprintInternalUseOnly = "true", DefaultToSelf = "OwningAbility", HidePin = "OwningAbility"))
	static UAbilityTask_InputBuffer* CreateInputBufferTask(
		UGameplayAbility* OwningAbility,
		UInputAction* InputAction,
		float BufferDuration = 0.2f // 默认缓冲时间200ms
	);

	// 任务激活入口
	virtual void Activate() override;

	// 输入事件回调
	void OnInputTriggered(const FInputActionInstance& Instance);

	// 广播缓冲结果
	void BroadcastBufferedAction();

	UPROPERTY(BlueprintAssignable)
	FInputBufferDelegate OnBufferedInput;	// 缓冲输入事件委托

protected:
	// 配置参数
	UPROPERTY()
	UInputAction* TargetInputAction;	// 目标输入动作

	float BufferTimeWindow;	// 缓冲时间窗口

	// 内部状态
	FTimerHandle BufferTimerHandle;	// 缓冲计时器句柄

	bool bHasBufferedInput = false;	// 是否有缓冲输入
	
};
