// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_InputBufferANS.generated.h"

class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FANSInputBufferDelegate, const UInputAction*, BufferedAction);

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UAbilityTask_InputBufferANS : public UAbilityTask
{
	GENERATED_BODY()

public:
	// 创建任务实例的静态方法
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Input Buffer", BlueprintInternalUseOnly = "true", DefaultToSelf = "OwningAbility", HidePin = "OwningAbility"))
	static UAbilityTask_InputBufferANS* CreateInputBufferTaskANS(
		UGameplayAbility* OwningAbility,
		FGameplayTag WindowTag // 关联到ANS使用的GameplayTag
	);

	// 任务激活入口
	virtual void Activate() override;

	// 任务结束入口
	virtual void OnDestroy(bool AbilityEnded) override;

	UPROPERTY(BlueprintAssignable)
	FANSInputBufferDelegate OnBufferedInput;	// 缓冲输入事件委托

	UPROPERTY()
	TArray<TObjectPtr<UInputAction>> TrackedInputActions; // 存储需要跟踪的输入动作

protected:
	// 输入事件回调
	void OnInputActionTriggered(const FInputActionInstance& Instance);
	
	// ANS窗口事件回调 严格匹配UE5.5的委托签名：使用指针参数
	void OnInputWindowOpened(const FGameplayEventData* EventData);
	void OnInputWindowClosed(const FGameplayEventData* EventData);

	// 当前缓冲的输入动作
	UPROPERTY()
	TArray<const UInputAction*> BufferedActions;

	// 关联的窗口标签
	UPROPERTY()
	FGameplayTag AssociatedWindowTag;
	
};
