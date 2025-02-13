// INVI_1998 All Rights Reserved.


#include "Component/Combat/PawnCombatComponent.h"


void UPawnCombatComponent::RegisterSpawnedWeapon(const FGameplayTag& InWeaponTag, AArcaneWeaponBase* InWeapon, bool bEquipped)
{
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTag), TEXT("Weapon %s already exists in the map!"), *InWeaponTag.ToString());
	check(InWeapon);

	CharacterCarriedWeaponMap.Emplace(InWeaponTag, InWeapon);

	if (bEquipped)
	{
		CurrentEquippedWeaponTag = InWeaponTag;
	}
}

AArcaneWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeapon(const FGameplayTag& InWeaponTag) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTag))
	{
		if (AArcaneWeaponBase* Weapon = CharacterCarriedWeaponMap.FindRef(InWeaponTag))
		{
			return Weapon;
		}
	}
	return nullptr;
}

AArcaneWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	return CurrentEquippedWeaponTag.IsValid() ? GetCharacterCarriedWeapon(CurrentEquippedWeaponTag) : nullptr;
}
