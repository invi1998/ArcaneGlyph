// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneBaseAnimInstance.h"
#include "ArcaneCharacterAnimInstance.generated.h"

class UCharacterMovementComponent;
class AArcaneCharacterBase;

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneCharacterAnimInstance : public UArcaneBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	// 该动画实例的更新函数是线程安全的，运行在独立的工作线程中，而非游戏线程中，因此可以在该函数中进行一些计算密集型的操作
	// 这意味着使用这个函数可以提高动画的性能，使用该函数来计算我们需要的动画数据是一个很大的优化项
	// 但是需要注意的是，该函数中不能访问任何非线程安全的数据，比如 Actor 的成员变量等
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	TObjectPtr<AArcaneCharacterBase> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> OwnerCharacterMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;	// 地面速度

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;	// 是否有加速度

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirection;	// 角色运动方向与面朝方向之间的水平平面角度差

private:
	
};
