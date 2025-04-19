// INVI_1998 All Rights Reserved.


#include "Items/Weapons/ArcaneSkeletonWeaponBase.h"


// Sets default values
AArcaneSkeletonWeaponBase::AArcaneSkeletonWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	SetRootComponent(WeaponSkeletalMesh);
	
}



