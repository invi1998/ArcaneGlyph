// INVI_1998 All Rights Reserved.


#include "DataAssets/StartupData/DataAsset_EnemyStartupDada.h"

#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/ArcaneEnemyGameplayAbility.h"

void UDataAsset_EnemyStartupDada::GiveToAbilitySystemComponent(UArcaneAbilitySystemComponent* InArcaneASC, int32 InApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InArcaneASC, InApplyLevel);

	if (EnemyCombatAbilities.Num() > 0)
	{
		for (const TSubclassOf<UArcaneEnemyGameplayAbility>& AbilityClass : EnemyCombatAbilities)
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec AbilitySpec(AbilityClass);
				AbilitySpec.Level = InApplyLevel;
				AbilitySpec.SourceObject = InArcaneASC->GetAvatarActor();
				InArcaneASC->GiveAbility(AbilitySpec);
			}
		}
	}
}
