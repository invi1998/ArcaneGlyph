// INVI_1998 All Rights Reserved.


#include "Component/Combat/HeroCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArcaneGameplayTags.h"
#include "Items/Weapons/ArcaneHeroWeapon.h"


AArcaneHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(const FGameplayTag& InWeaponTag) const
{
	return Cast<AArcaneHeroWeapon>(GetCharacterCarriedWeapon(InWeaponTag));
}

AArcaneHeroWeapon* UHeroCombatComponent::GetHeroCurrentEquippedWeapon() const
{
	return Cast<AArcaneHeroWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UHeroCombatComponent::GetHeroCurrenEquippedWeaponDamageAtLevel(float InLevel) const
{
	if (const AArcaneHeroWeapon* CurrentEquippedWeapon = GetHeroCurrentEquippedWeapon())
	{
		const FArcaneHeroWeaponData& WeaponData = CurrentEquippedWeapon->HeroWeaponData;
		return WeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
	}
	return 0.f;
}

void UHeroCombatComponent::OnHitTargetActor(AActor* InHitActor)
{
	// if (HitOverlappedActors.Contains(InHitActor)) return;
	// HitOverlappedActors.AddUnique(InHitActor);
	if (!IsValid(InHitActor)) return;
	HitOverlappedActors.Add(InHitActor);

	FGameplayEventData EventData;
	EventData.Target = InHitActor;
	EventData.Instigator = GetOwningPawn();

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		ArcaneGameplayTags::Shared_Event_MeleeAttack,
		EventData
	);

	// 发送受击暂停事件
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		ArcaneGameplayTags::Player_Event_HitPause,
		FGameplayEventData()
	);
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InHitActor)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		ArcaneGameplayTags::Player_Event_HitPause,
		FGameplayEventData()
	);
}
