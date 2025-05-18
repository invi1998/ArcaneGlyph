// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_InputBufferANS.generated.h"

class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FComboInputSignature, const UInputAction*, DetectedAction);

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UAbilityTask_InputBufferANS : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FComboInputSignature OnComboInputDetected;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UAbilityTask_InputBufferANS* ListenForComboInput(
		UGameplayAbility* OwningAbility,
		const TArray<UInputAction*>& ValidActions
	);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

private:
	UPROPERTY()
	TArray<TObjectPtr<UInputAction>> TrackedActions;

	// 存储每个输入动作的绑定句柄
	UPROPERTY()
	TMap<TObjectPtr<UInputAction>, uint32> InputBindings;

	void OnInputActionTriggered(const FInputActionInstance& ActionInstance);

	bool bIsFirstInput = true;	// 是否是第一次输入
	
};
