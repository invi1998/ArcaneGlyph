// INVI_1998 All Rights Reserved.


#include "AnimInstances/Hero/ArcaneHeroLinkedAnimLayer.h"

#include "AnimInstances/Hero/ArcaneHeroAnimInstance.h"

UArcaneHeroAnimInstance* UArcaneHeroLinkedAnimLayer::GetHeroAnimInstance() const
{
	return Cast<UArcaneHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}

void UArcaneHeroLinkedAnimLayer::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (UArcaneHeroAnimInstance* HeroAnimInstance = GetHeroAnimInstance())
	{
		CurrentGait = HeroAnimInstance->GetCurrentGait();
	}
}

