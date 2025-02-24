// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "ArcaneBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

protected:
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	bool DoesOwnerHaveTag(FGameplayTag InTag) const;
};
