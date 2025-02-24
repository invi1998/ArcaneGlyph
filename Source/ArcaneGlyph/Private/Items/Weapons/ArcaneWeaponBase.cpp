// INVI_1998 All Rights Reserved.


#include "Items/Weapons/ArcaneWeaponBase.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneDebugHelper.h"
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

	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AArcaneWeaponBase::OnWeaponCollisionBoxBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &AArcaneWeaponBase::OnWeaponCollisionBoxEndOverlap);
}

void AArcaneWeaponBase::OnWeaponCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	checkf(WeaponOwningPawn, TEXT("OnWeaponCollisionBoxBeginOverlap: Weapon (%s) must have a instigator pawn!"), *GetName());
	
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		// 如果目标是敌对的
		if (UArcaneBlueprintFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		{
			// 通知武器拥有者
			OnWeaponHitTarget.ExecuteIfBound(HitPawn);
		}
		
	}
}

void AArcaneWeaponBase::OnWeaponCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	checkf(WeaponOwningPawn, TEXT("OnWeaponCollisionBoxEndOverlap: Weapon (%s) must have a instigator pawn!"), *GetName());
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		// 如果目标是敌对的
		if (UArcaneBlueprintFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		{
			// 通知武器拥有者
			OnWeaponPulledTarget.ExecuteIfBound(HitPawn);
		}
	}
}

void AArcaneWeaponBase::ToggleWeaponCollision_Implementation(bool bEnable)
{
	WeaponCollisionBox->SetCollisionEnabled(bEnable ? ECollisionEnabled::Type::QueryOnly : ECollisionEnabled::Type::NoCollision);
}



