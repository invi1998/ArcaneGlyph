// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ArcaneAbilitySystemComponent.h"

void UArcaneAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (InInputTag.IsValid())
	{
		// ActivatableAbilities.Items
		for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
		{
			if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag))
			{
				// 说明这个技能是通过这个输入标签激活的
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UArcaneAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	
}
