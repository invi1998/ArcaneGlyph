// INVI_1998 All Rights Reserved.


#include "Component/Combat/HeroCombatComponent.h"

#include "Items/Weapons/ArcaneHeroWeapon.h"


AArcaneHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(const FGameplayTag& InWeaponTag) const
{
	return Cast<AArcaneHeroWeapon>(GetCharacterCarriedWeapon(InWeaponTag));
}

void UHeroCombatComponent::OnHitTargetActor(AActor* InHitActor)
{
	Super::OnHitTargetActor(InHitActor);
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InHitActor)
{
	Super::OnWeaponPulledFromTargetActor(InHitActor);
}
