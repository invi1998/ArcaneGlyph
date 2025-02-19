// INVI_1998 All Rights Reserved.


#include "Items/Weapons/ArcaneWeaponBase.h"

#include "Components/BoxComponent.h"


AArcaneWeaponBase::AArcaneWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);		// 默认关闭碰撞
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);					// 默认忽略所有碰撞

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f, 20.f, 20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);		// 默认关闭碰撞

	// 忽略摄像机碰撞
	WeaponCollisionBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
}

void AArcaneWeaponBase::ToggleWeaponCollision_Implementation(bool bEnable)
{
	WeaponCollisionBox->SetCollisionEnabled(bEnable ? ECollisionEnabled::Type::QueryOnly : ECollisionEnabled::Type::NoCollision);
}



