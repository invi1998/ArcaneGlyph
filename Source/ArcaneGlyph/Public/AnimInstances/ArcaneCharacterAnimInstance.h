// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneBaseAnimInstance.h"
#include "ArcaneTypes/ArcaneEnumTypes.h"
#include "ArcaneTypes/ArcaneStructTypes.h"
#include "Interfaces/ArcaneGaitDataInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "ArcaneCharacterAnimInstance.generated.h"

class UCharacterMovementComponent;
class AArcaneCharacterBase;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneCharacterAnimInstance : public UArcaneBaseAnimInstance, public IArcaneGaitDataInterface
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void UpdateHipFacingByCurve();
	void UpdateRootYawOffsetData(float DeltaSeconds);
	void SetRootYawOffset(float InRootYawOffset);

	// 该动画实例的更新函数是线程安全的，运行在独立的工作线程中，而非游戏线程中，因此可以在该函数中进行一些计算密集型的操作
	// 这意味着使用这个函数可以提高动画的性能，使用该函数来计算我们需要的动画数据是一个很大的优化项
	// 但是需要注意的是，该函数中不能访问任何非线程安全的数据，比如 Actor 的成员变量等
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	virtual void ReceiveGaitData_Implementation(const EArcaneGaits InGait) override;

	FORCEINLINE float GetLocomotionDirection() const { return LocomotionDirectionAngle; }
	FORCEINLINE EArcaneGaits GetCurrentGait() const { return CurrentGait; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FORCEINLINE bool IsMoving() const { return Speed != 0.0f; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FORCEINLINE bool IsNotMoving() const { return Speed == 0.0f; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FORCEINLINE float GetYawSpeed() const { return YawSpeed; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FORCEINLINE float GetSmoothedYawSpeed() const { return SmoothedYawSpeed; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FORCEINLINE bool IsJumping() const { return bIsJumping; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FORCEINLINE bool IsOnGround() const { return !bIsJumping; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FORCEINLINE float GetLookYawOffset() const { return LookRotOffset.Yaw; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FORCEINLINE float GetLookPitchOffset() const { return LookRotOffset.Pitch; }
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FORCEINLINE bool GetIsAlive() const { return bIsAlive; }

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|ReferenceData")
	TObjectPtr<AArcaneCharacterBase> OwnerCharacter;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|ReferenceData")
	TObjectPtr<UCharacterMovementComponent> OwnerCharacterMovementComponent;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|ReferenceData")
	bool bIsAlive;	// 角色是否存活

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	FVector WorldVelocity;	// 角色在世界坐标系中的速度

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;	// 地面速度

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;	// 是否有加速度

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirectionAngle;	// 角色运动方向与面朝方向之间的水平平面角度差，这是一个-180~180的值

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionAngleWithOffset;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float AccelerationLocomotionAngle;		// 加速度方向与面朝方向之间的水平平面角度差，这是一个-180~180的值

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	EArcaneMoveDirection CurrentLocomotionDirection;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	EArcaneMoveDirection AccelerationLocomotionDirection;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	EArcaneMoveDirection PreviousLocomotionDirection;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	EArcaneMoveDirection PreviousAccelerationLocomotionDirection;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	EArcaneLocomotionDirection LocomotionDirection;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "AnimData|LocomotionData")
	EArcaneHipFacing HipFacingDirection;	// 角色的臀部朝向方向

	// Unreal Engine 的 PropertyAccess 系统（用于动态访问属性和函数）依赖 反射（Reflection） 生成的元数据来识别函数返回值。
	// 当你在蓝图中定义一个函数时，引擎会为函数的输入/输出参数生成元数据。默认情况下，返回值的名称被强制标记为 ReturnValue。
	// PropertyAccess 在查找函数返回值时，会严格按照反射元数据中定义的名称（ReturnValue）进行匹配。
	// 如果你手动修改了返回值的名称（例如改为 Result 或 Output），反射元数据中的返回值名称将不再匹配 ReturnValue，导致 PropertyAccess 无法识别。

	// 角色世界坐标
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|LocationData")
	FVector ArcaneWorldLocation;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|LocationData")
	FVector PreviousWorldLocation;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|LocationData")
	float LocomotionDelta;	// 角色在世界坐标系中的位移
	
	// 角色世界旋转
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|RotationData")
	FRotator ArcaneWorldRotation;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|RotationData")
	float PreviousActorYaw;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|RotationData")
	float CurrentActorYaw;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|RotationData")
	float ActorYawDelta;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|RotationData")
	float LeanAngle;		// 角色的倾斜角度
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|RootYawOffsetData")
	float RootYawOffset = 0.f;	// 角色的根部偏移角度

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|RootYawOffsetData")
	float PreviousRootYawOffset;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|RootYawOffsetData")
	FFloatSpringState RootYawOffsetSpringState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimData|RootYawOffsetData")
	EArcaneRootYawOffsetMode RootYawOffsetMode;
	
	// 角色加速度
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|VelocityData")
	FVector  ArcaneAcceleration;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|VelocityData")
	FVector ArcaneAcceleration2D;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "AnimData|VelocityData")
	FVector PivotAcceleration2D;

	// 角色的水平速度和垂直速度
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AnimData|VelocityData")
	FVector Velocity2D;

	// 角色步态
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|GaitData")
	EArcaneGaits InComingGait;
	
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|GaitData")
	EArcaneGaits CurrentGait;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|GaitData")
	EArcaneGaits PreviousGait;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|GaitData")
	bool bGaitChanged;

	EArcaneMoveDirection CalculateLocomotionDirection(float Angle, EArcaneMoveDirection Direction, const FArcaneLocomotionDirectionSettings& InSettings);

	EArcaneLocomotionDirection CalculateLocomotionDirection4D(float Angle, const EArcaneLocomotionDirection& CurrentDirection, const FArcaneLocomotionDirectionSettings_4D& InSettings);

	/*
	 * 跳跃相关
	 */
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|JumpData")
	bool bIsOnGround;		// 是否在地面上

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|JumpData")
	bool bIsJumping;		// 是否正在跳跃

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|JumpData")
	bool bIsFalling;		// 是否正在下落

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|JumpData")
	float TimeToJumpApex;	// 跳跃顶点时间

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "AnimData|JumpData")
	float GroundDistance;	// 距离地面的距离

	void UpdateJumpFallData();
	void UpdateGroundDistance();
	
protected:

	// 辅助函数1：检查角度是否在某个方向范围内（含死区扩展）
	bool IsAngleInDirectionWithDeadZone(float Angle, EArcaneMoveDirection Direction, const FArcaneLocomotionDirectionSettings& InSettings) const;

	// 辅助函数2：通用角度范围检查（处理环形角度）
	bool IsAngleInRange(float Angle, float Min, float Max) const;

	// 辅助函数：判断是否为向后移动
	bool IsBackwardMovement() const;

	// 辅助函数：判断方向是否属于"前向树"
	bool IsForwardTree(EArcaneMoveDirection Direction) const;
	bool IsBackwardTree(EArcaneMoveDirection Direction) const;

	// 核心臀部朝向更新逻辑
	void UpdateHipFacingDirection(EArcaneMoveDirection PreviousDir, EArcaneMoveDirection NewDir);

	float Speed;
	float YawSpeed;
	float SmoothedYawSpeed;		// 平滑的身体旋转角速度

	UPROPERTY(EditAnywhere, Category = "Animation")
	float YawSpeedSmoothLerpSpeed = 1.f;	// 平滑的身体旋转角速度的插值速度
	
	FRotator BodyPrevRot;	// 角色身体的上一个旋转角度
	FRotator LookRotOffset;	// 角色头部的旋转偏移

	
};
