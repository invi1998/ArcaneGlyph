// INVI_1998 All Rights Reserved.


#include "AnimInstances/ArcaneCharacterAnimInstance.h"

#include "Characters/ArcaneCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	bHasAcceleration = OwnerCharacterMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.0f;
}
