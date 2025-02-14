// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/ArcaneGameplayAbility.h"

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

void UArcaneAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FArcaneHeroAbilitySet>& InAbilitySets, int32 InApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InAbilitySets.Num() > 0)
	{
		for (const FArcaneHeroAbilitySet& AbilitySet : InAbilitySets)
		{
			if (AbilitySet.IsValid())
			{
				// 生成一个新的动态能力规格
				FGameplayAbilitySpec NewAbilitySpec(AbilitySet.AbilityToGrantClass);
				NewAbilitySpec.SourceObject = GetAvatarActor();
				NewAbilitySpec.Level = InApplyLevel;
				NewAbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);

				// 给予能力
				GiveAbility(NewAbilitySpec);

				OutGrantedAbilitySpecHandles.AddUnique(NewAbilitySpec.Handle);
			}
		}
	}
}
