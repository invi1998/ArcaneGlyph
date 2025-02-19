// INVI_1998 All Rights Reserved.


#include "DataAssets/StartupData/DataAsset_StartupDadaBase.h"

#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/ArcaneGameplayAbility.h"

void UDataAsset_StartupDadaBase::GiveToAbilitySystemComponent(UArcaneAbilitySystemComponent* InArcaneASC, int32 InApplyLevel)
{
	check(InArcaneASC);
	GrantAbilities(ActivateOnGivenAbilityList, InArcaneASC, InApplyLevel);
	GrantAbilities(ReactiveAbilityList, InArcaneASC, InApplyLevel);

	// 应用启动游戏效果
	if (!StartupGameplayEffects.IsEmpty())
    {
        for (const TSubclassOf<UGameplayEffect>& GameplayEffectClass : StartupGameplayEffects)
        {
            if (GameplayEffectClass)
            {
                FGameplayEffectContextHandle EffectContext = InArcaneASC->MakeEffectContext();
                EffectContext.AddSourceObject(InArcaneASC->GetAvatarActor());
                InArcaneASC->ApplyGameplayEffectToSelf(GameplayEffectClass.GetDefaultObject(), InApplyLevel, EffectContext);
            }
        }
    }
}

void UDataAsset_StartupDadaBase::GrantAbilities(const TArray<TSubclassOf<UArcaneGameplayAbility>>& InAbilityList, UArcaneAbilitySystemComponent* InArcaneASC, int32 InApplyLevel)
{
	if (InAbilityList.IsEmpty()) return;

	for (const TSubclassOf<UArcaneGameplayAbility>& Ability : InAbilityList)
	{
		if (Ability)
		{
			FGameplayAbilitySpec AbilitySpec(Ability);
			AbilitySpec.Level = InApplyLevel;
			AbilitySpec.SourceObject = InArcaneASC->GetAvatarActor();
			
			InArcaneASC->GiveAbility(AbilitySpec);
		}
	}
}
