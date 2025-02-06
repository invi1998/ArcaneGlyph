// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DadaAsset_InputConfig.h"
#include "ArcaneInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<typename UserObject, typename CallbackFunction>
	void BindNativeInputAction(const UDadaAsset_InputConfig* InInputConfig, const FGameplayTag& InTag, ETriggerEvent InEvent, UserObject* InContextObject, CallbackFunction InCallbackFunction)
	{
		checkf(InInputConfig, TEXT("InputConfig is nullptr, cannot bind native input action"));
		if (UInputAction* InputAction = InInputConfig->GetNativeInputActionByTag(InTag))
		{
			BindAction(InputAction, InEvent, InContextObject, InCallbackFunction);
		}
	}
	
};
