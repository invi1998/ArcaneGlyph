// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FrontendLoadScreenInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UFrontendLoadScreenInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCANEGLYPH_API IFrontendLoadScreenInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void OnLoadingScreenActivated();

	UFUNCTION(BlueprintNativeEvent)
	void OnLoadingScreenDeactivated();
	
};
