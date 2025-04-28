// INVI_1998 All Rights Reserved.


#include "AnimInstances/ArcaneBaseAnimInstance.h"

#include "ArcaneBlueprintFunctionLibrary.h"

#include "Characters/ArcaneCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UArcaneBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<AArcaneCharacterBase>(TryGetPawnOwner());
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UArcaneBaseAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!IsValid(OwnerCharacter) || !IsValid(OwnerCharacterMovementComponent))
	{
		return;
	}

	GroundSpeed = OwnerCharacterMovementComponent->Velocity.Size2D();
	// 计算当前移动的方向
	LocomotionDirectionAngle = UKismetAnimationLibrary::CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetActorRotation());
	// 计算当前角色位置
	ArcaneWorldLocation = OwnerCharacter->GetActorLocation();
	// 计算当前角色Rotator
	ArcaneWorldRotation = OwnerCharacter->GetActorRotation();
	// 计算当前角色的加速度
	ArcaneAcceleration = OwnerCharacterMovementComponent->GetCurrentAcceleration();
	bHasAcceleration = ArcaneAcceleration.SizeSquared2D() > 0.0f;

	CurrentLocomotionDirection = CalculateLocomotionDirection(FArcaneLocomotionDirectionSettings());
}

bool UArcaneBaseAnimInstance::ReceiveGaitData(const EArcaneGaits InGait)
{
	CurrentGait = InGait;
	return true;
}

EArcaneMoveDirection UArcaneBaseAnimInstance::CalculateLocomotionDirection(const FArcaneLocomotionDirectionSettings& InSettings) const
{
	if (!bHasAcceleration)
	{
		return EArcaneMoveDirection::None;
	}

	// 规范化角度到[-180°, 180°]
	const float NormalizedAngle = FRotator::NormalizeAxis(LocomotionDirectionAngle);

	// 步骤1：检查是否在死区内保持当前方向
	if (CurrentLocomotionDirection != EArcaneMoveDirection::None)
	{
		const bool bIsInDeadZone = IsAngleInDirectionWithDeadZone(
			NormalizedAngle, CurrentLocomotionDirection, InSettings);
		if (bIsInDeadZone)
		{
			return CurrentLocomotionDirection;
		}
	}

	// 步骤2：按优先级重新判定方向（从最小区间开始检查）
	if (IsAngleInRange(NormalizedAngle, InSettings.FMin, InSettings.FMax))
	{
		return EArcaneMoveDirection::Forward;
	}
	else if (IsAngleInRange(NormalizedAngle, InSettings.FRMin, InSettings.FRMax))
	{
		return EArcaneMoveDirection::ForwardRight;
	}
	else if (IsAngleInRange(NormalizedAngle, InSettings.RMin, InSettings.RMax))
	{
		return EArcaneMoveDirection::Right;
	}
	else if (IsAngleInRange(NormalizedAngle, InSettings.BRMin, InSettings.BRMax))
	{
		return EArcaneMoveDirection::BackwardRight;
	}
	else if (IsAngleInRange(NormalizedAngle, InSettings.BLMin, InSettings.BLMax))
	{
		return EArcaneMoveDirection::BackwardLeft;
	}
	else if (IsAngleInRange(NormalizedAngle, InSettings.LMin, InSettings.LMax))
	{
		return EArcaneMoveDirection::Left;
	}
	else if (IsAngleInRange(NormalizedAngle, InSettings.FLMin, InSettings.FLMax))
	{
		return EArcaneMoveDirection::ForwardLeft;
	}
	else if (IsAngleInRange(NormalizedAngle, InSettings.BMin, InSettings.BMax))
	{
		return EArcaneMoveDirection::Backward;
	}

	return EArcaneMoveDirection::None;
	
}

bool UArcaneBaseAnimInstance::DoesOwnerHaveTag(FGameplayTag InTag) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UArcaneBlueprintFunctionLibrary::NativeDoesActorHasGameplayTag(OwningPawn, InTag);
	}

	return false;
}

bool UArcaneBaseAnimInstance::IsAngleInDirectionWithDeadZone(float Angle, EArcaneMoveDirection Direction, const FArcaneLocomotionDirectionSettings& InSettings) const
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

bool UArcaneBaseAnimInstance::IsAngleInRange(float Angle, float Min, float Max) const
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
