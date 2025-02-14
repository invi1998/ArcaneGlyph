// INVI_1998 All Rights Reserved.


#include "DataAssets/StartupData/UDataAsset_HeroStartupDada.h"

#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/ArcaneGameplayAbility.h"

void UUDataAsset_HeroStartupDada::GiveToAbilitySystemComponent(UArcaneAbilitySystemComponent* InArcaneASC, int32 InApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InArcaneASC, InApplyLevel);

	for (const FArcaneHeroAbilitySet& AbilitySet : HeroStartupAbilitySets)
	{
		if (AbilitySet.IsValid())
		{
			FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrantClass);
			AbilitySpec.SourceObject = InArcaneASC->GetAvatarActor();
			AbilitySpec.Level = InApplyLevel;
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
			InArcaneASC->GiveAbility(AbilitySpec);
		}
	}
	
}
