// INVI_1998 All Rights Reserved.


#include "Component/Combat/PawnCombatComponent.h"

#include "Items/Weapons/ArcaneWeaponBase.h"


void UPawnCombatComponent::RegisterSpawnedWeapon(const FGameplayTag& InWeaponTag, AArcaneWeaponBase* InWeapon, bool bEquipped)
{
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTag), TEXT("Weapon %s already exists in the map!"), *InWeaponTag.ToString());
	check(InWeapon);

	CharacterCarriedWeaponMap.Emplace(InWeaponTag, InWeapon);

	// 因为我们的武器击中委托是原生多播委托，所以绑定的函数可以不必是UFUNCTION
	InWeapon->OnWeaponHitTarget.BindUObject(this, &UPawnCombatComponent::OnHitTargetActor);
	InWeapon->OnWeaponPulledTarget.BindUObject(this, &UPawnCombatComponent::OnWeaponPulledFromTargetActor);

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

void UPawnCombatComponent::ToggleWeaponCollision(bool bEnable, EToggleDamageType InToggleDamageType)
{
	if (InToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		if (AArcaneWeaponBase* CurrentEquippedWeapon = GetCharacterCurrentEquippedWeapon())
		{
			CurrentEquippedWeapon->ToggleWeaponCollision(bEnable);
		}

		if (!bEnable)
		{
			HitOverlappedActors.Empty();
		}
	}
}

void UPawnCombatComponent::OnHitTargetActor(AActor* InHitActor)
{
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InHitActor)
{
}
