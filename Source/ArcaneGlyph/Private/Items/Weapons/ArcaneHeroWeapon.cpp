// INVI_1998 All Rights Reserved.


#include "Items/Weapons/ArcaneHeroWeapon.h"

#include "Components/BoxComponent.h"


// Sets default values
AArcaneHeroWeapon::AArcaneHeroWeapon(): HeroWeaponData()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	SetRootComponent(WeaponSkeletalMesh);
	WeaponCollisionBox1->SetupAttachment(GetRootComponent());
}

void AArcaneHeroWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles)
{
	GrantedAbilitySpecHandles = InGrantedAbilitySpecHandles;
}
