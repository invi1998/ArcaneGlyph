// INVI_1998 All Rights Reserved.


#include "AbilitySystem/Abilities/HeroGameplayAbility_TargetLock.h"

#include "ArcaneDebugHelper.h"
#include "Characters/ArcaneHeroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widget/ArcaneWidgetBase.h"
#include "Controllers/ArcaneHeroController.h"


void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                      const FGameplayEventData* TriggerEventData)
{
	TryLockTargetLock();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Cleanup();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::TryLockTargetLock()
{
	GetAvailableTargetToLock();

	if (AvailableTargetToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	CurrentLockedActor = GetNearestTargetFromAvailable(AvailableTargetToLock);
	if (CurrentLockedActor)
	{
		DrawTargetLockWidget();
	}
	else
	{
		CancelTargetLockAbility();
	}
	
}

void UHeroGameplayAbility_TargetLock::GetAvailableTargetToLock()
{

	TArray<FHitResult> BoxTraceHitResults;
	
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetHeroCharacterFromActorInfo(),		// 从ActorInfo获取英雄角色
		GetHeroCharacterFromActorInfo()->GetActorLocation(),		// 从英雄角色获取位置
		GetHeroCharacterFromActorInfo()->GetActorLocation() + GetHeroCharacterFromActorInfo()->GetActorForwardVector() * LockDistance,		// 从英雄角色获取前方1000米的位置
		TraceBoxSize * 0.5f,		// 射线盒大小
		GetHeroCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),		// 从英雄角色获取前方向量
		TargetLockObjectTypes,		// 指定检测目标的对象类型
		false,		// 是否检测复杂碰撞
		{},		// 忽略的Actor
		bShowDebugTrace ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,		// 是否显示调试射线
		BoxTraceHitResults,		// 射线命中结果
		true		// 是否忽略自身
		);

	// 遍历射线命中结果
	for (const FHitResult& HitResult : BoxTraceHitResults)
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (HitActor != GetHeroCharacterFromActorInfo())
			{
				AvailableTargetToLock.AddUnique(HitActor);

			}
		}
	}
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestTargetFromAvailable(const TArray<AActor*> InAvailableActors)
{
	float ClosestDistance = TNumericLimits<float>::Max();

	// 使用GameplayStatics的FindNearestActor函数来查找最近的Actor，该函数会返回最近的Actor和距离
	return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, ClosestDistance);
	
}

void UHeroGameplayAbility_TargetLock::DrawTargetLockWidget()
{
	if (!IsValid(TargetLockWidget))
	{
		checkf(TargetLockWidgetClass, TEXT("TargetLockWidgetClass is not set in %s!"), *GetName());

		TargetLockWidget = CreateWidget<UArcaneWidgetBase>(GetHeroControllerFromActorInfo(), TargetLockWidgetClass);

		check(TargetLockWidget);
	}
	
	TargetLockWidget->AddToViewport();
}

void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroGameplayAbility_TargetLock::Cleanup()
{
	AvailableTargetToLock.Empty();
	CurrentLockedActor = nullptr;

	if (IsValid(TargetLockWidget))
	{
		TargetLockWidget->RemoveFromParent();
	}
}
