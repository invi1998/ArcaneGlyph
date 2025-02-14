// INVI_1998 All Rights Reserved.


#include "Component/Combat/HeroCombatComponent.h"

#include "Items/Weapons/ArcaneHeroWeapon.h"


AArcaneHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(const FGameplayTag& InWeaponTag) const
{
	return Cast<AArcaneHeroWeapon>(GetCharacterCarriedWeapon(InWeaponTag));
}
