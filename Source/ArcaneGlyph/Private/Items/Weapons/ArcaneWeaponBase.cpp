// INVI_1998 All Rights Reserved.


#include "Items/Weapons/ArcaneWeaponBase.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "Components/BoxComponent.h"


AArcaneWeaponBase::AArcaneWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);		// 默认关闭碰撞
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);					// 默认忽略所有碰撞

	WeaponCollisionBox1 = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox1->SetupAttachment(GetRootComponent());
	WeaponCollisionBox1->SetBoxExtent(FVector(20.f, 20.f, 20.f));
	WeaponCollisionBox1->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);		// 默认关闭碰撞

	// 忽略摄像机碰撞
	WeaponCollisionBox1->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	WeaponCollisionBox1->OnComponentBeginOverlap.AddUniqueDynamic(this, &AArcaneWeaponBase::OnWeaponCollisionBoxBeginOverlap);
	WeaponCollisionBox1->OnComponentEndOverlap.AddUniqueDynamic(this, &AArcaneWeaponBase::OnWeaponCollisionBoxEndOverlap);

	WeaponCollisionBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox2"));
	WeaponCollisionBox2->SetupAttachment(GetRootComponent());
	WeaponCollisionBox2->SetBoxExtent(FVector(20.f, 20.f, 20.f));
	WeaponCollisionBox2->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);		// 默认关闭碰撞

	// 忽略摄像机碰撞
	WeaponCollisionBox2->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	if (bShouldUseTwoCollisionBoxes)
	{
		WeaponCollisionBox2->OnComponentBeginOverlap.AddUniqueDynamic(this, &AArcaneWeaponBase::OnWeaponCollisionBoxBeginOverlap);
		WeaponCollisionBox2->OnComponentEndOverlap.AddUniqueDynamic(this, &AArcaneWeaponBase::OnWeaponCollisionBoxEndOverlap);
	}
	
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
			// 获取碰撞盒子的名字，用于判断是哪个碰撞盒子
			FName CollisionBoxName = OverlappedComponent->GetFName();
			if (CollisionBoxName == WeaponCollisionBox1->GetFName())
			{
				// 通知武器拥有者
				OnWeaponHitTarget.ExecuteIfBound(HitPawn, 1);
			}
			else if (CollisionBoxName == WeaponCollisionBox2->GetFName())
			{
				// 通知武器拥有者
				OnWeaponHitTarget.ExecuteIfBound(HitPawn, 2);
			}
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
			OnWeaponPulledTarget.ExecuteIfBound(HitPawn, 0);
		}
	}
}

void AArcaneWeaponBase::ToggleWeaponCollision_Implementation(bool bEnable)
{
	WeaponCollisionBox1->SetCollisionEnabled(bEnable ? ECollisionEnabled::Type::QueryOnly : ECollisionEnabled::Type::NoCollision);

	if (bShouldUseTwoCollisionBoxes)
	{
		WeaponCollisionBox2->SetCollisionEnabled(bEnable ? ECollisionEnabled::Type::QueryOnly : ECollisionEnabled::Type::NoCollision);
	}
}



