// INVI_1998 All Rights Reserved.


#include "DataAssets/Input/DadaAsset_InputConfig.h"

UInputAction* UDadaAsset_InputConfig::GetNativeInputActionByTag(const FGameplayTag& InTag) const
{
	for (const FArcaneInputActionConfig& ActionConfig : NativeInputActions)
	{
		if (ActionConfig.InputTag.MatchesTag(InTag))
		{
			return ActionConfig.InputAction;
		}
	}

	return nullptr;
}
