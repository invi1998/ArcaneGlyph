// INVI_1998 All Rights Reserved.


#include "AnimInstances/Hero/ArcaneHeroAnimInstance.h"

#include "Characters/ArcaneHeroCharacter.h"


void UArcaneHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (IsValid(OwnerCharacter))
	{
		OwnerHeroCharacter = Cast<AArcaneHeroCharacter>(OwnerCharacter);
	}
}

void UArcaneHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bHasAcceleration)
	{
		IdleElapsedTime = 0.f;
		bShouldEnterRelaxedState = false;
	}
	else
	{
		IdleElapsedTime += DeltaSeconds;
		if (IdleElapsedTime >= EnterRelaxedStateTime)
		{
			bShouldEnterRelaxedState = true;
		}
		else
		{
			bShouldEnterRelaxedState = false;
		}
	}
}
