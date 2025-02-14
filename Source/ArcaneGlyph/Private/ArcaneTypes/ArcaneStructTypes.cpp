// INVI_1998 All Rights Reserved.


#include "ArcaneTypes/ArcaneStructTypes.h"

#include "AbilitySystem/Abilities/ArcaneGameplayAbility.h"

bool FArcaneHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrantClass;
}
