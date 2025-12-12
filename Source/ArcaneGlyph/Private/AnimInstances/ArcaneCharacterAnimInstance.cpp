// INVI_1998 All Rights Reserved.


#include "AnimInstances/ArcaneCharacterAnimInstance.h"

#include "Characters/ArcaneCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "ArcaneTypes/ArcaneEnumTypes.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SpringInterpolator.h"
#include "Kismet/KismetMathLibrary.h"

void UArcaneCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<AArcaneCharacterBase>(TryGetPawnOwner());
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UArcaneCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (OwnerCharacterMovementComponent)
	{
		Speed = OwnerCharacter->GetVelocity().Length();
		FRotator BodyRot = OwnerCharacter->GetActorRotation();
		FRotator BodyRotDelta = UKismetMathLibrary::NormalizedDeltaRotator(BodyRot, BodyPrevRot);	// 计算身体旋转的增量，同时该增量会被规范化到0-180度范围内
		BodyPrevRot = BodyRot;

		YawSpeed = BodyRotDelta.Yaw / DeltaSeconds;	// 计算身体旋转的角速度

		// 因为我们使用鼠标控制旋转移动的时候，会导致身体旋转角速度的变化非常大，使用这种方法会有些不稳定
		// 我们会看到身体动作变得很急促，所以我们需要平滑处理身体旋转角速度
		// 这里我们使用插值来平滑处理身体旋转角速度
		// 这里的插值速度可以根据需要进行调整，默认值为1
		SmoothedYawSpeed = UKismetMathLibrary::FInterpTo(SmoothedYawSpeed, YawSpeed, DeltaSeconds, YawSpeedSmoothLerpSpeed);

		// GetBaseAimRotation 若存在控制器，默认情况下会瞄准玩家的视线方向（即摄像机朝向）
		FRotator ControlRot = OwnerCharacter->GetBaseAimRotation();
		// 若存在控制器，默认情况下我们会瞄准玩家视线方向（即摄像机朝向）正对着的方向。
		// 这样就很完美了。
		// 对于pawn来说，默认情况下当它的视角旋转时就是这样的。
		// 但如果是人眼视角的摄像机，效果会更好。不过这只是从实现层面来看。这有点奇怪。
		// 我们来看看这里的实现。
		// 它试图获取控制器。
		// 如果控制器不为空，那么它就不是空的。然后我们只需让控制器提供视点位置，也就是摄像机的位置。没问题。我们就返回这个值。
		// 不过，若没有控制器，我们将采用角色的旋转角度——这样处理也很不错 对于 AI 而言。
		// 但随后会出现这种奇怪的俯仰角更新问题。
		// 因此当俯仰角为零时，我们会用同步复制的视角俯仰角来更新它。
		// 我不明白他们为何要采用这种方式，因为你看。
		// 他们确实有一个名为 remoteViewPitch 的变量，并且这个变量是同步复制的，对吧？
		// 但他们没有同步复制远程视角偏转。这会导致问题，因为在多人游戏中我们将只有同步的俯仰角数据。
		// 这样就不会同步偏航角了，对吧？所以在多人游戏中这个功能无法 100%生效。
		// 只有俯仰角会生效，因为在多人游戏中控制器并不存在于其他客户端上，对吧？
		// 这意味着只有我们的客户端能看到俯仰角在起作用，但偏航角没有效果

		LookRotOffset = UKismetMathLibrary::NormalizedDeltaRotator(ControlRot, BodyRot);	// 计算头部旋转的偏移量
		
	}

	if (OwnerCharacterMovementComponent)
	{
		bIsJumping = OwnerCharacterMovementComponent->IsFalling();
	}
}

void UArcaneCharacterAnimInstance::UpdateHipFacingByCurve()
{
	// 设置HipFacing
	const float HipFacingValue = GetCurveValue(FName("HipFacing"));
	if (HipFacingValue > 0)
	{
		HipFacingDirection = EArcaneHipFacing::Forward;
	}
	else if (HipFacingValue < 0)
	{
		HipFacingDirection = EArcaneHipFacing::Backward;
	}
}

void UArcaneCharacterAnimInstance::UpdateRootYawOffsetData(float DeltaSeconds)
{
	if (RootYawOffsetMode == EArcaneRootYawOffsetMode::Accumulate)
	{
		// 当我们处于累积根部偏航偏移量（AccumulateRootYawOffset）时，我们将把根部Yaw偏移量叠加到当前的偏移值上
		// 这样，在我们自由旋转摄像机时，我们的角色网格体仍能保持朝向特定的方向
		SetRootYawOffset(RootYawOffset - ActorYawDelta);
		
	}
	if (RootYawOffsetMode == EArcaneRootYawOffsetMode::BlendOut)
	{
		// 单我们处于淡出根部偏航偏移量（BlendOutRootYawOffset）时，我们将把根部Yaw偏移量淡出到0（随时间推移将那个根 Y 轴偏移值混合归零）
		const float Offset = UKismetMathLibrary::FloatSpringInterp(
			RootYawOffset,
			0.f,
			RootYawOffsetSpringState,
			80.f,
			1.f,
			DeltaSeconds,
			1.f,
			0.5f);

		SetRootYawOffset(Offset);
		
	}
	if (RootYawOffsetMode == EArcaneRootYawOffsetMode::Hold)
	{
		// 当我们处于保持根部偏航偏移量（HoldRootYawOffset）时，我们将把根部Yaw偏移量保持在当前的偏移值上
		
		RootYawOffset = FMath::FInterpTo(RootYawOffset, 0.f, DeltaSeconds, 5.f);
	}
}

void UArcaneCharacterAnimInstance::SetRootYawOffset(float InRootYawOffset)
{
	PreviousRootYawOffset = RootYawOffset;
	InRootYawOffset = FRotator::NormalizeAxis(InRootYawOffset);
	RootYawOffset = InRootYawOffset;
}

void UArcaneCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!IsValid(OwnerCharacter) || !IsValid(OwnerCharacterMovementComponent))
	{
		return;
	}
	
	bIsAlive = OwnerCharacter->IsAlive();

	WorldVelocity = OwnerCharacterMovementComponent->Velocity;
	GroundSpeed = OwnerCharacterMovementComponent->Velocity.Size2D();
	Velocity2D = OwnerCharacterMovementComponent->Velocity * FVector(1, 1, 0);

	// 计算当前角色Rotator
	PreviousActorYaw = CurrentActorYaw;
	ArcaneWorldRotation = OwnerCharacter->GetActorRotation();
	CurrentActorYaw = ArcaneWorldRotation.Yaw;
	ActorYawDelta = (CurrentActorYaw - PreviousActorYaw);

	// 计算当前角色的加速度
	ArcaneAcceleration = OwnerCharacterMovementComponent->GetCurrentAcceleration();
	ArcaneAcceleration2D = ArcaneAcceleration * FVector(1, 1, 0);
	
	// 计算当前移动的方向
	LocomotionDirectionAngle = UKismetAnimationLibrary::CalculateDirection(Velocity2D, ArcaneWorldRotation);
	LocomotionAngleWithOffset = FRotator::NormalizeAxis(LocomotionDirectionAngle - RootYawOffset);
	AccelerationLocomotionAngle = UKismetAnimationLibrary::CalculateDirection(ArcaneAcceleration2D, OwnerCharacter->GetActorRotation());

	PreviousWorldLocation = ArcaneWorldLocation;
	// 计算当前角色位置
	ArcaneWorldLocation = OwnerCharacter->GetActorLocation();
	// 计算角色的起始距离
	LocomotionDelta = (ArcaneWorldLocation - PreviousWorldLocation).Length();
	
	
	bHasAcceleration = UKismetMathLibrary::NearlyEqual_FloatFloat(ArcaneAcceleration.SizeSquared2D(), 0.f, 0.001f) == false;
	// bHasAcceleration = UKismetMathLibrary::NearlyEqual_FloatFloat(Velocity2D.Length(), 0.f, 0.001f) == false;

	// 记录上一帧方向用于过渡判断
	PreviousLocomotionDirection = CurrentLocomotionDirection;
	CurrentLocomotionDirection = CalculateLocomotionDirection(LocomotionDirectionAngle, CurrentLocomotionDirection, FArcaneLocomotionDirectionSettings());
	PreviousAccelerationLocomotionDirection = AccelerationLocomotionDirection;
	AccelerationLocomotionDirection = CalculateLocomotionDirection(AccelerationLocomotionAngle, AccelerationLocomotionDirection, FArcaneLocomotionDirectionSettings());

	UpdateHipFacingByCurve();

	float LeanAngleFactor = HipFacingDirection == EArcaneHipFacing::Forward ? 1.f : -1.f;

	LeanAngle = FMath::Clamp(UKismetMathLibrary::SafeDivide(ActorYawDelta, DeltaSeconds)/4.f * LeanAngleFactor, -90.f, 90.f);
	
	// LocomotionDirection = CalculateLocomotionDirection4D(LocomotionDirectionAngle, LocomotionDirection, FArcaneLocomotionDirectionSettings_4D());

	PreviousGait = CurrentGait;
	CurrentGait = InComingGait;
	bGaitChanged = InComingGait != PreviousGait;

	UpdateRootYawOffsetData(DeltaSeconds);

	
	
}

void UArcaneCharacterAnimInstance::ReceiveGaitData_Implementation(const EArcaneGaits InGait)
{
	InComingGait = InGait;
}

bool UArcaneCharacterAnimInstance::ShouldDoFullBody() const
{
	return (GetSpeed() <= 0) && !bIsJumping;
}

EArcaneMoveDirection UArcaneCharacterAnimInstance::CalculateLocomotionDirection(float Angle, EArcaneMoveDirection Direction, const FArcaneLocomotionDirectionSettings& InSettings)
{
	/*
	if (!bHasAcceleration)
	{
		HipFacingDirection = EArcaneHipFacing::Forward;
		return EArcaneMoveDirection::None;
	}
	*/

	// 规范化角度到[-180°, 180°]
	const float NormalizedAngle = FRotator::NormalizeAxis(Angle);
	// const float NormalizedAngle = LocomotionDirectionAngle;

	EArcaneMoveDirection NewDirection = Direction;

	// 步骤1：检查是否在死区内保持当前方向
	if (IsAngleInDirectionWithDeadZone(NormalizedAngle, Direction, InSettings))
	{
		return NewDirection;
	}
	else
	{
		// 步骤2：按优先级重新判定方向（从最小区间开始检查）
		// 按优先级重新判定方向
		if (IsAngleInRange(NormalizedAngle, InSettings.FMin, InSettings.FMax))
		{
			NewDirection = EArcaneMoveDirection::Forward;
			HipFacingDirection = EArcaneHipFacing::Forward;
		}
		else if (IsAngleInRange(NormalizedAngle, InSettings.FRMin, InSettings.FRMax))  NewDirection = EArcaneMoveDirection::ForwardRight;
		else if (IsAngleInRange(NormalizedAngle, InSettings.RMin, InSettings.RMax))
		{
			NewDirection = EArcaneMoveDirection::Right;
			
			if (IsForwardTree(PreviousLocomotionDirection))
			{
				HipFacingDirection = EArcaneHipFacing::Forward;
			}
			else if (IsBackwardTree(PreviousLocomotionDirection))
			{
				HipFacingDirection = EArcaneHipFacing::Backward;
			}
			else if (PreviousLocomotionDirection == EArcaneMoveDirection::Left)
			{
				HipFacingDirection = HipFacingDirection == EArcaneHipFacing::Forward ? EArcaneHipFacing::Backward : EArcaneHipFacing::Forward;
			}
			
			
		}
		else if (IsAngleInRange(NormalizedAngle, InSettings.BRMin, InSettings.BRMax))  NewDirection = EArcaneMoveDirection::BackwardRight;
		else if (IsAngleInRange(NormalizedAngle, InSettings.BLMin, InSettings.BLMax))  NewDirection = EArcaneMoveDirection::BackwardLeft;
		else if (IsAngleInRange(NormalizedAngle, InSettings.LMin, InSettings.LMax))
		{
			NewDirection = EArcaneMoveDirection::Left;
			
			if (IsForwardTree(PreviousLocomotionDirection))
			{
				HipFacingDirection = EArcaneHipFacing::Forward;
			}
			else if (IsBackwardTree(PreviousLocomotionDirection))
			{
				HipFacingDirection = EArcaneHipFacing::Backward;
			}
			else if (PreviousLocomotionDirection == EArcaneMoveDirection::Right)
			{
				HipFacingDirection = HipFacingDirection == EArcaneHipFacing::Forward ? EArcaneHipFacing::Backward : EArcaneHipFacing::Forward;
			}
			
		}
		else if (IsAngleInRange(NormalizedAngle, InSettings.FLMin, InSettings.FLMax))  NewDirection = EArcaneMoveDirection::ForwardLeft;
		else if (IsAngleInRange(NormalizedAngle, InSettings.BMin, InSettings.BMax))
		{
			NewDirection = EArcaneMoveDirection::Backward;
			HipFacingDirection = EArcaneHipFacing::Backward;
		}
	}

	//--- 步骤2：动态更新臀部朝向 ---
	// UpdateHipFacingDirection(PreviousDirection, NewDirection);
	return NewDirection;
	
}

EArcaneLocomotionDirection UArcaneCharacterAnimInstance::CalculateLocomotionDirection4D(float Angle, const EArcaneLocomotionDirection& CurrentDirection, const FArcaneLocomotionDirectionSettings_4D& InSettings)
{
	switch (CurrentDirection) {
	case EArcaneLocomotionDirection::Forward:
		{
			if (IsAngleInRange(Angle,InSettings.FMin - InSettings.DeadZone, InSettings.FMax + InSettings.DeadZone))
				return EArcaneLocomotionDirection::Forward;
		}
		break;
	case EArcaneLocomotionDirection::Backward:
		{
			if (IsAngleInRange(Angle, InSettings.BMin + InSettings.DeadZone, InSettings.BMax - InSettings.DeadZone))
				return EArcaneLocomotionDirection::Backward;
		}
		break;
	case EArcaneLocomotionDirection::Left:
		{
			if (IsAngleInRange(Angle, InSettings.BMin - InSettings.DeadZone, InSettings.FMin + InSettings.DeadZone))
				return EArcaneLocomotionDirection::Left;
		}
		break;
	case EArcaneLocomotionDirection::Right:
		{
			if (IsAngleInRange(Angle, InSettings.FMax - InSettings.DeadZone, InSettings.BMax + InSettings.DeadZone))
				return EArcaneLocomotionDirection::Right;
		}
		break;
	}
	
	if (!IsAngleInRange(Angle, InSettings.BMin, InSettings.BMax)) return EArcaneLocomotionDirection::Backward;
	else if (IsAngleInRange(Angle, InSettings.FMin, InSettings.FMax)) return EArcaneLocomotionDirection::Forward;
	else if (Angle < 0.f) return EArcaneLocomotionDirection::Left;
	else return EArcaneLocomotionDirection::Right;
}

void UArcaneCharacterAnimInstance::UpdateJumpFallData()
{
	if (bIsJumping)
	{
		// 获取角色跳跃到顶点的时间（V = gt, t = V/g, 因为跳跃时Z轴速度为负，所以需要取负值）
		TimeToJumpApex = 0.f - WorldVelocity.Z / OwnerCharacterMovementComponent->GetGravityZ();
	}
	else
	{
		TimeToJumpApex = 0.f;
	}
}

void UArcaneCharacterAnimInstance::UpdateGroundDistance()
{
	if (bIsFalling)
	{
		FHitResult HitResult;
		// 使用线性追踪来检测地面距离
		const FVector Start = OwnerCharacter->GetActorLocation();
		const FVector End = Start + FVector(0.f, 0.f, -10000.f) - FVector(0.f, 0.f, OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(OwnerCharacter);
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
		{
			// 计算地面距离
			GroundDistance = HitResult.Distance;
		}
		else
		{
			GroundDistance = 0.f; // 如果没有检测到地面，则设置为0
		}
	}
}

bool UArcaneCharacterAnimInstance::IsAngleInDirectionWithDeadZone(float Angle, EArcaneMoveDirection Direction, const FArcaneLocomotionDirectionSettings& InSettings) const
{
	switch (Direction)
	{
	case EArcaneMoveDirection::Forward:
		return IsAngleInRange(Angle, InSettings.FMin - InSettings.DeadZone, InSettings.FMax + InSettings.DeadZone);
	case EArcaneMoveDirection::ForwardRight:
		return IsAngleInRange(Angle, InSettings.FRMin - InSettings.DeadZone, InSettings.FRMax + InSettings.DeadZone);
	case EArcaneMoveDirection::Right:
		return IsAngleInRange(Angle, InSettings.RMin - InSettings.DeadZone, InSettings.RMax + InSettings.DeadZone);
	case EArcaneMoveDirection::BackwardRight:
		return IsAngleInRange(Angle, InSettings.BRMin - InSettings.DeadZone, InSettings.BRMax + InSettings.DeadZone);
	case EArcaneMoveDirection::Backward:
		return IsAngleInRange(Angle, InSettings.BMin - InSettings.DeadZone, InSettings.BMax + InSettings.DeadZone);
	case EArcaneMoveDirection::BackwardLeft:
		return IsAngleInRange(Angle, InSettings.BLMin - InSettings.DeadZone, InSettings.BLMax + InSettings.DeadZone);
	case EArcaneMoveDirection::Left:
		return IsAngleInRange(Angle, InSettings.LMin - InSettings.DeadZone, InSettings.LMax + InSettings.DeadZone);
	case EArcaneMoveDirection::ForwardLeft:
		return IsAngleInRange(Angle, InSettings.FLMin - InSettings.DeadZone, InSettings.FLMax + InSettings.DeadZone);
	default:
		return false;
	}
}

bool UArcaneCharacterAnimInstance::IsAngleInRange(float Angle, float Min, float Max) const
{
	// 处理Backward的特殊环形范围（如157.5°~-157.5°）
	if (Min > Max)
	{
		return (Angle >= Min || Angle <= Max);
	}
	else
	{
		return (Angle >= Min && Angle <= Max);
	}
}

bool UArcaneCharacterAnimInstance::IsBackwardMovement() const
{
	return (CurrentLocomotionDirection == EArcaneMoveDirection::Backward ||
				CurrentLocomotionDirection == EArcaneMoveDirection::BackwardLeft ||
				CurrentLocomotionDirection == EArcaneMoveDirection::BackwardRight);
}

// 辅助函数：判断方向是否属于"前向树"
bool UArcaneCharacterAnimInstance::IsForwardTree(EArcaneMoveDirection Direction) const
{
	switch (Direction)
	{
	case EArcaneMoveDirection::Forward:
	case EArcaneMoveDirection::ForwardRight:
	case EArcaneMoveDirection::ForwardLeft:
		return true;
	default:
		return false; // Backward系列和None属于后向树
	}
}

bool UArcaneCharacterAnimInstance::IsBackwardTree(EArcaneMoveDirection Direction) const
{
	switch (Direction)
	{
	case EArcaneMoveDirection::Backward:
	case EArcaneMoveDirection::BackwardRight:
	case EArcaneMoveDirection::BackwardLeft:
		return true;
	default:
		return false; // Forward系列和None属于前向树
	}
}

// 核心臀部朝向更新逻辑
void UArcaneCharacterAnimInstance::UpdateHipFacingDirection(EArcaneMoveDirection PreviousDir, EArcaneMoveDirection NewDir)
{
	// 初始化处理：首次移动时根据方向设定初始朝向
	if (PreviousDir == EArcaneMoveDirection::None)
	{
		HipFacingDirection = IsForwardTree(NewDir) ? EArcaneHipFacing::Forward : EArcaneHipFacing::Backward;
		return;
	}
	
	// 方向未变化时保持当前朝向
	if (PreviousDir == NewDir) return;

	// 判断方向树是否切换
	const bool bWasForward = IsForwardTree(PreviousDir);
	const bool bIsNowForward = IsForwardTree(NewDir);

	if (bWasForward != bIsNowForward)
	{
		// 方向树切换时翻转臀部朝向
		HipFacingDirection = (HipFacingDirection == EArcaneHipFacing::Forward) ? 
							EArcaneHipFacing::Backward : 
							EArcaneHipFacing::Forward;
	}
}


