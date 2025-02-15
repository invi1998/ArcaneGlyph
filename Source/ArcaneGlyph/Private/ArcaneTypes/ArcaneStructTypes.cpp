// INVI_1998 All Rights Reserved.


#include "ArcaneTypes/ArcaneStructTypes.h"

#include "AbilitySystem/Abilities/ArcaneHeroGameplayAbility.h"

bool FArcaneHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrantClass;
}
