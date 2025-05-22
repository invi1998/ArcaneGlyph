// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ArcaneStatusInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UArcaneStatusInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCANEGLYPH_API IArcaneStatusInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Arcane | Status")
	void OnSparkChanged(const int32 InSparkValue);
};
