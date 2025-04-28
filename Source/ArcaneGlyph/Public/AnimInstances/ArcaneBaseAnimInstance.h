// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "ArcaneTypes/ArcaneEnumTypes.h"
#include "ArcaneTypes/ArcaneStructTypes.h"
#include "Interfaces/ArcaneGaitDataInterface.h"
#include "ArcaneBaseAnimInstance.generated.h"

class AArcaneCharacterBase;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneBaseAnimInstance : public UAnimInstance, public IArcaneGaitDataInterface
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	// 该动画实例的更新函数是线程安全的，运行在独立的工作线程中，而非游戏线程中，因此可以在该函数中进行一些计算密集型的操作
	// 这意味着使用这个函数可以提高动画的性能，使用该函数来计算我们需要的动画数据是一个很大的优化项
	// 但是需要注意的是，该函数中不能访问任何非线程安全的数据，比如 Actor 的成员变量等
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	virtual bool ReceiveGaitData(const EArcaneGaits InGait) override;

	FORCEINLINE float GetLocomotionDirection() const { return LocomotionDirectionAngle; }

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
	float LocomotionDirectionAngle;	// 角色运动方向与面朝方向之间的水平平面角度差，这是一个-180~180的值

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	EArcaneMoveDirection CurrentLocomotionDirection;

	// Unreal Engine 的 PropertyAccess 系统（用于动态访问属性和函数）依赖 反射（Reflection） 生成的元数据来识别函数返回值。
	// 当你在蓝图中定义一个函数时，引擎会为函数的输入/输出参数生成元数据。默认情况下，返回值的名称被强制标记为 ReturnValue。
	// PropertyAccess 在查找函数返回值时，会严格按照反射元数据中定义的名称（ReturnValue）进行匹配。
	// 如果你手动修改了返回值的名称（例如改为 Result 或 Output），反射元数据中的返回值名称将不再匹配 ReturnValue，导致 PropertyAccess 无法识别。

	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "AnimData|LocationData")
	FVector ArcaneWorldLocation;		// 角色世界坐标

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "AnimData|RotationData")
	FRotator ArcaneWorldRotation;		// 角色世界旋转

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "AnimData|AccelerationData")
	FVector  ArcaneAcceleration;		// 角色加速度
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "AnimData|VelocityData")
	EArcaneGaits CurrentGait = EArcaneGaits::Walking;		// 角色步态
	
	EArcaneMoveDirection CalculateLocomotionDirection(const FArcaneLocomotionDirectionSettings& InSettings) const;

	

protected:
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	bool DoesOwnerHaveTag(FGameplayTag InTag) const;

	// 辅助函数1：检查角度是否在某个方向范围内（含死区扩展）
	bool IsAngleInDirectionWithDeadZone(float Angle, EArcaneMoveDirection Direction, const FArcaneLocomotionDirectionSettings& InSettings) const;

	// 辅助函数2：通用角度范围检查（处理环形角度）
	bool IsAngleInRange(float Angle, float Min, float Max) const;



};
