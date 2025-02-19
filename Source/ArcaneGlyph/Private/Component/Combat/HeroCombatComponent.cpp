// INVI_1998 All Rights Reserved.


#include "Component/Combat/HeroCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArcaneGameplayTags.h"
#include "Items/Weapons/ArcaneHeroWeapon.h"


AArcaneHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(const FGameplayTag& InWeaponTag) const
{
	return Cast<AArcaneHeroWeapon>(GetCharacterCarriedWeapon(InWeaponTag));
}

void UHeroCombatComponent::OnHitTargetActor(AActor* InHitActor)
{
	if (HitOverlappedActors.Contains(InHitActor)) return;
	HitOverlappedActors.AddUnique(InHitActor);

	FGameplayEventData EventData;
	EventData.Target = InHitActor;
	EventData.Instigator = GetOwningPawn();

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		ArcaneGameplayTags::Shared_Event_MeleeAttack,
		EventData
	);
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InHitActor)
{
}
