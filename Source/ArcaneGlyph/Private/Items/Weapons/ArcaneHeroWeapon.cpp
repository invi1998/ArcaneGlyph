// INVI_1998 All Rights Reserved.


#include "Items/Weapons/ArcaneHeroWeapon.h"


// Sets default values
AArcaneHeroWeapon::AArcaneHeroWeapon(): HeroWeaponData()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AArcaneHeroWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles)
{
	GrantedAbilitySpecHandles = InGrantedAbilitySpecHandles;
}
