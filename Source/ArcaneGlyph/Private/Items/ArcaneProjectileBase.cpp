// INVI_1998 All Rights Reserved.


#include "Items/ArcaneProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneGameplayTags.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AArcaneProjectileBase::AArcaneProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
	SetRootComponent(ProjectileCollisionBox);
	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	// 添加碰撞盒子的碰撞事件
	ProjectileCollisionBox->OnComponentHit.AddUniqueDynamic(this, &AArcaneProjectileBase::OnProjectileHit);
	ProjectileCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AArcaneProjectileBase::OnProjectileBeginOverlap);

	ProjectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComponent"));
	ProjectileNiagaraComponent->SetupAttachment(GetRootComponent());
	ProjectileNiagaraComponent->SetAutoActivate(false);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 700.f;
	ProjectileMovementComponent->MaxSpeed = 1900.f;
	ProjectileMovementComponent->Velocity = FVector(1.f, 0, 0);		// 让投射物朝向X轴正方向
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;		// 重力缩放

	InitialLifeSpan = 2.f;		// 默认2秒后销毁
	
}

void AArcaneProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileDamagePolicy == EProjectileDamagePolicy::OnOverlap)
	{
		ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	
}

void AArcaneProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	BP_OnSpawnProjectileHitFX(Hit.ImpactPoint);
	APawn* HitPawn = Cast<APawn>(OtherActor);
	
	// 如果目标不是敌对的，或者目标不是Pawn，就不做任何事情
	// || UArcaneBlueprintFunctionLibrary::IsTargetPawnHostile(GetInstigator<APawn>(), HitPawn)
	if (!HitPawn)
	{
		Destroy();
		return;
	}

	bool bIsValidBlock = false;
	const bool bIsPlayerBlocking = UArcaneBlueprintFunctionLibrary::NativeDoesActorHasGameplayTag(HitPawn, ArcaneGameplayTags::Player_Status_Blocking);

	if (bIsPlayerBlocking)
	{
		bIsValidBlock = UArcaneBlueprintFunctionLibrary::IsCurrentBlockValid(GetInstigator<APawn>(), HitPawn);
	}

	FGameplayEventData EventData;
	EventData.Target = HitPawn;
	EventData.Instigator = this;

	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			ArcaneGameplayTags::Player_Event_BlockSuccess,
			EventData
		);
	}
	else
	{
		// apply damage
	}

	Destroy();
	
}

void AArcaneProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}



