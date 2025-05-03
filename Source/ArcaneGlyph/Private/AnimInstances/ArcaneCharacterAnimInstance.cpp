// INVI_1998 All Rights Reserved.


#include "AnimInstances/ArcaneCharacterAnimInstance.h"

#include "Characters/ArcaneCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "ArcaneTypes/ArcaneEnumTypes.h"
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

void UArcaneCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!IsValid(OwnerCharacter) || !IsValid(OwnerCharacterMovementComponent))
	{
		return;
	}

	GroundSpeed = OwnerCharacterMovementComponent->Velocity.Size2D();
	Velocity2D = OwnerCharacterMovementComponent->Velocity * FVector(1, 1, 0);

	// 计算当前角色Rotator
	PreviousActorYaw = CurrentActorYaw;
	ArcaneWorldRotation = OwnerCharacter->GetActorRotation();
	CurrentActorYaw = ArcaneWorldRotation.Yaw;
	ActorYawDelta = (CurrentActorYaw - PreviousActorYaw);
	
	// 计算当前移动的方向
	// LocomotionDirectionAngle = UKismetAnimationLibrary::CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetActorRotation());
	LocomotionDirectionAngle = UKismetAnimationLibrary::CalculateDirection(Velocity2D, ArcaneWorldRotation);
	// 计算当前角色位置
	ArcaneWorldLocation = OwnerCharacter->GetActorLocation();
	
	// 计算当前角色的加速度
	ArcaneAcceleration = OwnerCharacterMovementComponent->GetCurrentAcceleration();
	
	bHasAcceleration = UKismetMathLibrary::NearlyEqual_FloatFloat(ArcaneAcceleration.SizeSquared2D(), 0.f, 0.001f) == false;
	// bHasAcceleration = UKismetMathLibrary::NearlyEqual_FloatFloat(Velocity2D.Length(), 0.f, 0.001f) == false;

	CurrentLocomotionDirection = CalculateLocomotionDirection(FArcaneLocomotionDirectionSettings());

	float LeanAngleFactor = HipFacingDirection == EArcaneHipFacing::Forward ? 1.f : -1.f;

	LeanAngle = FMath::Clamp(UKismetMathLibrary::SafeDivide(ActorYawDelta, DeltaSeconds)/4.f * LeanAngleFactor, -90.f, 90.f);
	
	// LocomotionDirection = CalculateLocomotionDirection4D(LocomotionDirectionAngle, LocomotionDirection, FArcaneLocomotionDirectionSettings_4D());
}

void UArcaneCharacterAnimInstance::ReceiveGaitData_Implementation(const EArcaneGaits InGait)
{
	CurrentGait = InGait;
}

EArcaneMoveDirection UArcaneCharacterAnimInstance::CalculateLocomotionDirection(const FArcaneLocomotionDirectionSettings& InSettings)
{
	/*
	if (!bHasAcceleration)
	{
		HipFacingDirection = EArcaneHipFacing::Forward;
		return EArcaneMoveDirection::None;
	}
	*/

	// 规范化角度到[-180°, 180°]
	const float NormalizedAngle = FRotator::NormalizeAxis(LocomotionDirectionAngle);
	// const float NormalizedAngle = LocomotionDirectionAngle;

	// 记录上一帧方向用于过渡判断
	PreviousLocomotionDirection = CurrentLocomotionDirection;

	//--- 步骤1：计算当前方向 ---
	EArcaneMoveDirection NewDirection = PreviousLocomotionDirection;

	// 步骤1：检查是否在死区内保持当前方向
	if (IsAngleInDirectionWithDeadZone(NormalizedAngle, CurrentLocomotionDirection, InSettings))
	{
		NewDirection = CurrentLocomotionDirection;
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


