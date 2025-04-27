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

EArcaneMoveDirection UArcaneBaseAnimInstance::CalculateLocomotionDirection(FArcaneLocomotionDirectionSettings InLocomotionDirectionSettings) const
{
	switch (CurrentLocomotionDirection)
	{
	case EArcaneMoveDirection::None:
		break;
	case EArcaneMoveDirection::Forward:
		{
			if (LocomotionDirectionAngle >= InLocomotionDirectionSettings.FMin &&
				LocomotionDirectionAngle <= InLocomotionDirectionSettings.FMax)
			{
				return EArcaneMoveDirection::Forward;
			}
		}
		break;
	case EArcaneMoveDirection::ForwardRight:
		{
			if (LocomotionDirectionAngle >= InLocomotionDirectionSettings.FRMin &&
				LocomotionDirectionAngle <= InLocomotionDirectionSettings.FRMax)
			{
				return EArcaneMoveDirection::ForwardRight;
			}
		}
		break;
	case EArcaneMoveDirection::Right:
		{
			if (LocomotionDirectionAngle >= InLocomotionDirectionSettings.RMin &&
				LocomotionDirectionAngle <= InLocomotionDirectionSettings.RMax)
			{
				return EArcaneMoveDirection::Right;
			}
		}
		break;
	case EArcaneMoveDirection::BackwardRight:
		if (LocomotionDirectionAngle >= InLocomotionDirectionSettings.BRMin &&
			LocomotionDirectionAngle <= InLocomotionDirectionSettings.BRMax)
		{
			return EArcaneMoveDirection::BackwardRight;
		}
		break;
	case EArcaneMoveDirection::Backward:
		{
			if (!(LocomotionDirectionAngle >= InLocomotionDirectionSettings.BMin + InLocomotionDirectionSettings.DeadZone &&
				LocomotionDirectionAngle <= InLocomotionDirectionSettings.BMax - InLocomotionDirectionSettings.DeadZone))
			{
				return EArcaneMoveDirection::Backward;
			}
		}
		break;
	case EArcaneMoveDirection::BackwardLeft:
		if (LocomotionDirectionAngle >= InLocomotionDirectionSettings.BLMin &&
			LocomotionDirectionAngle <= InLocomotionDirectionSettings.BLMax)
		{
			return EArcaneMoveDirection::BackwardLeft;
		}
		break;
	case EArcaneMoveDirection::Left:
		if (LocomotionDirectionAngle >= InLocomotionDirectionSettings.LMin &&
			LocomotionDirectionAngle <= InLocomotionDirectionSettings.LMax)
		{
			return EArcaneMoveDirection::Left;
		}
		break;
	case EArcaneMoveDirection::ForwardLeft:
		if (LocomotionDirectionAngle >= InLocomotionDirectionSettings.FLMin &&
			LocomotionDirectionAngle <= InLocomotionDirectionSettings.FLMax)
		{
			return EArcaneMoveDirection::ForwardLeft;
		}
		break;
	}

	if (!(LocomotionDirectionAngle >= InLocomotionDirectionSettings.BMin + InLocomotionDirectionSettings.DeadZone &&
				LocomotionDirectionAngle <= InLocomotionDirectionSettings.BMax - InLocomotionDirectionSettings.DeadZone))
	{
		return EArcaneMoveDirection::Backward;
	}
	else if (LocomotionDirectionAngle >= InLocomotionDirectionSettings.BLMin &&
				LocomotionDirectionAngle <= InLocomotionDirectionSettings.BLMax)
	{
		return EArcaneMoveDirection::BackwardLeft;
	}
	else if (LocomotionDirectionAngle >= InLocomotionDirectionSettings.LMin &&
				LocomotionDirectionAngle <= InLocomotionDirectionSettings.LMax)
	{
		return EArcaneMoveDirection::Left;
	}
	else if (LocomotionDirectionAngle >= InLocomotionDirectionSettings.FLMin &&
				LocomotionDirectionAngle <= InLocomotionDirectionSettings.FLMax)
	{
		return EArcaneMoveDirection::ForwardLeft;
	}
	else if (LocomotionDirectionAngle >= InLocomotionDirectionSettings.FMin &&
				LocomotionDirectionAngle <= InLocomotionDirectionSettings.FMax)
	{
		return EArcaneMoveDirection::Forward;
	}
	else if (LocomotionDirectionAngle >= InLocomotionDirectionSettings.FRMin &&
				LocomotionDirectionAngle <= InLocomotionDirectionSettings.FRMax)
	{
		return EArcaneMoveDirection::ForwardRight;
	}
	else if (LocomotionDirectionAngle >= InLocomotionDirectionSettings.RMin &&
				LocomotionDirectionAngle <= InLocomotionDirectionSettings.RMax)
	{
		return EArcaneMoveDirection::Right;
	}
	else if (LocomotionDirectionAngle >= InLocomotionDirectionSettings.BRMin &&
				LocomotionDirectionAngle <= InLocomotionDirectionSettings.BRMax)
	{
		return EArcaneMoveDirection::BackwardRight;
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
