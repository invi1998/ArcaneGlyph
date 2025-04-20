// INVI_1998 All Rights Reserved.


#include "Items/Weapons/ArcaneHeroWeapon.h"


// Sets default values
AArcaneHeroWeapon::AArcaneHeroWeapon(): HeroWeaponData()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	SetRootComponent(WeaponSkeletalMesh);
}

void AArcaneHeroWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles)
{
	GrantedAbilitySpecHandles = InGrantedAbilitySpecHandles;
}
