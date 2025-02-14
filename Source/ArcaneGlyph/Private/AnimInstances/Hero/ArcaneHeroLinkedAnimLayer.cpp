// INVI_1998 All Rights Reserved.


#include "AnimInstances/Hero/ArcaneHeroLinkedAnimLayer.h"

#include "AnimInstances/Hero/ArcaneHeroAnimInstance.h"

UArcaneHeroAnimInstance* UArcaneHeroLinkedAnimLayer::GetHeroAnimInstance() const
{
	return Cast<UArcaneHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
