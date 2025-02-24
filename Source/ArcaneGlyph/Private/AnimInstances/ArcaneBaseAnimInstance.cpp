// INVI_1998 All Rights Reserved.


#include "AnimInstances/ArcaneBaseAnimInstance.h"

#include "ArcaneBlueprintFunctionLibrary.h"

bool UArcaneBaseAnimInstance::DoesOwnerHaveTag(FGameplayTag InTag) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UArcaneBlueprintFunctionLibrary::NativeDoesActorHasGameplayTag(OwningPawn, InTag);
	}

	return false;
}
