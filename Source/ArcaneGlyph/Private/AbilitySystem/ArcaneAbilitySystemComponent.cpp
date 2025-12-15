// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ArcaneAbilitySystemComponent.h"

#include "ArcaneGameplayTags.h"
#include "AbilitySystem/ArcaneAttributeSet.h"
#include "AbilitySystem/Abilities/ArcaneHeroGameplayAbility.h"
#include "Items/Weapons/ArcaneHeroWeapon.h"

UArcaneAbilitySystemComponent::UArcaneAbilitySystemComponent()
{
	GetGameplayAttributeValueChangeDelegate(UArcaneAttributeSet::GetCurrentHealthAttribute()).AddUObject(this, &UArcaneAbilitySystemComponent::OnCurrentHealthChanged);
}

void UArcaneAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (InInputTag.IsValid())
	{
		// ActivatableAbilities.Items
		for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
		{
			if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag))
			{
				if (InInputTag.MatchesTag(ArcaneGameplayTags::InputTag_Toggleable) && AbilitySpec.IsActive())
				{
					// 如果当前技能是未激活状态，就激活
					CancelAbilityHandle(AbilitySpec.Handle);
				}
				else
				{
					// 说明这个技能是通过这个输入标签激活的
					TryActivateAbility(AbilitySpec.Handle);
				}
			}
		}
	}
}

void UArcaneAbilitySystemComponent::SendChargeComplete()
{
	// 向Ability发送事件Tag
	FGameplayEventData EventData;
	EventData.EventTag = ArcaneGameplayTags::Player_Event_Charge_Complete;
	EventData.Instigator = GetAvatarActor();
	EventData.Target = GetAvatarActor();

	// 发送事件
	HandleGameplayEvent(ArcaneGameplayTags::Player_Event_Charge_Complete, &EventData);
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
			if (InInputTag.MatchesTag(ArcaneGameplayTags::InputTag_MustBeHeld_Combo_Heavy))
			{
				SendChargeComplete();
			}
			else
			{
				// 如果当前输入指向的技能是出于激活状态，就取消激活
				CancelAbilityHandle(AbilitySpec.Handle);
			}
		}
	}
	
}

void UArcaneAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FArcaneHeroAbilitySet>& InAbilitySets, const TArray<FArcaneHeroSpecialAbilitySet>& InSpecialAbilities, int32 InApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
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

	if (OutGrantedAbilitySpecHandles.Num() > 0)
	{
		for (const FArcaneHeroSpecialAbilitySet& SpecialAbilitySet : InSpecialAbilities)
		{
			if (SpecialAbilitySet.IsValid())
			{
				FGameplayAbilitySpec NewAbilitySpec(SpecialAbilitySet.AbilityToGrantClass);
				NewAbilitySpec.SourceObject = GetAvatarActor();
				NewAbilitySpec.Level = InApplyLevel;
				NewAbilitySpec.GetDynamicSpecSourceTags().AddTag(SpecialAbilitySet.InputTag);

				GiveAbility(NewAbilitySpec);

				OutGrantedAbilitySpecHandles.AddUnique(NewAbilitySpec.Handle);
				
			}
		}
		
	}
}

void UArcaneAbilitySystemComponent::GrantHeroComboGroupAbilities(const TArray<FArcaneHeroAbilitySet>& InAbilitySetGroups, int32 InApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedComboGroupAbilitySpecHandles)
{
	if (InAbilitySetGroups.Num() > 0)
	{
		for (const FArcaneHeroAbilitySet& AbilitySet : InAbilitySetGroups)
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
				
				OutGrantedComboGroupAbilitySpecHandles.AddUnique(NewAbilitySpec.Handle);
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

void UArcaneAbilitySystemComponent::RemoveGrantHeroComboGroupAbilities(AArcaneHeroWeapon* InWeapon)
{
	if (InWeapon)
	{
		TArray<FGameplayAbilitySpecHandle>& GrantedComboGroupAbilitySpecHandles = InWeapon->GetGrantedComboGroupAbilitySpecHandles();
		for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : GrantedComboGroupAbilitySpecHandles)
		{
			// 移除能力
			ClearAbility(AbilitySpecHandle);
		}

		GrantedComboGroupAbilitySpecHandles.Empty();
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

void UArcaneAbilitySystemComponent::OnCurrentHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (!GetOwner()) return;
	
	if (HasMatchingGameplayTag(ArcaneGameplayTags::Shared_Status_Dead))
	{
		OnActorDeathDelegate.Broadcast(GetOwner());
		return;
	}
	
	if (OnAttributeChangeData.NewValue <= 0.f)
	{
		// 当前Health值小于等于0，触发死亡逻辑
		// AddLooseGameplayTag(ArcaneGameplayTags::Shared_Status_Dead);

		OnActorDeathDelegate.Broadcast(GetOwner());
	}
	
}



