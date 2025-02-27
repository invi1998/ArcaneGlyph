// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ArcaneAbilitySystemComponent.h"

#include "ArcaneDebugHelper.h"
#include "ArcaneGameplayTags.h"
#include "AbilitySystem/Abilities/ArcaneHeroGameplayAbility.h"
#include "Items/Weapons/ArcaneHeroWeapon.h"

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
	if (!InInputTag.IsValid() || !InInputTag.MatchesTag(ArcaneGameplayTags::InputTag_MustBeHeld))
	{
		return;
	}

	// 能走到下面的，就是那些持续按住的输入标签
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag) && AbilitySpec.IsActive())
		{
			// 如果当前输入指向的技能是出于激活状态，就取消激活
			CancelAbilityHandle(AbilitySpec.Handle);
		}
	}
	
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

				Debug::Print(FString::Printf(TEXT("Grant Ability: %s"), *AbilitySet.AbilityToGrantClass->GetName()));

				OutGrantedAbilitySpecHandles.AddUnique(NewAbilitySpec.Handle);
			}
		}
	}
}

void UArcaneAbilitySystemComponent::RemoveGrantHeroWeaponAbilities(AArcaneHeroWeapon* InWeapon)
{
	if (InWeapon)
	{
		TArray<FGameplayAbilitySpecHandle>& GrantedAbilitySpecHandles = InWeapon->GetGrantedAbilitySpecHandles();
		for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : GrantedAbilitySpecHandles)
		{
			// 移除能力
			ClearAbility(AbilitySpecHandle);
		}

		GrantedAbilitySpecHandles.Empty();
	}
}

bool UArcaneAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag InAbilityTag)
{
	check(InAbilityTag.IsValid());

	TArray<FGameplayAbilitySpec*> MatchingAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(InAbilityTag.GetSingleTagContainer(), MatchingAbilitySpecs);

	if (!MatchingAbilitySpecs.IsEmpty())
	{
		const int32 RandomAbilityIndex = FMath::RandRange(0, MatchingAbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* AbilitySpecChoose = MatchingAbilitySpecs[RandomAbilityIndex];
		check(AbilitySpecChoose);
		if (!AbilitySpecChoose->IsActive())
		{
			return TryActivateAbility(AbilitySpecChoose->Handle);
		}
	}

	return false;
}

