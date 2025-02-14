// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/ArcaneBaseAnimInstance.h"
#include "ArcaneHeroLinkedAnimLayer.generated.h"

class UArcaneHeroAnimInstance;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneHeroLinkedAnimLayer : public UArcaneBaseAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	UArcaneHeroAnimInstance* GetHeroAnimInstance() const;
};
