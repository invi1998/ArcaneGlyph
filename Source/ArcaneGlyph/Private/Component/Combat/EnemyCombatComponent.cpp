// INVI_1998 All Rights Reserved.


#include "Component/Combat/EnemyCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArcaneDebugHelper.h"
#include "ArcaneGameplayTags.h"


void UEnemyCombatComponent::OnHitTargetActor(AActor* InHitActor)
{
	if (!InHitActor) return;
	
	if (HitOverlappedActors.Contains(InHitActor))
	{
		// 说明已经击中过了
		return;
	}

	HitOverlappedActors.AddUnique(InHitActor);

	// TODO: 格挡机制后续实现
	const bool bIsValidBlock = false;	// 是否有效的格挡
	const bool bIsPlayerBlocking = false;	// 玩家是否正在格挡
	const bool bIsMyAttackUnblockable = false;	// 我的攻击是否无法格挡

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		// TODO：判定格挡是否成功
		
		
	}

	FGameplayEventData EventData;
	EventData.Target = InHitActor;
	EventData.Instigator = GetOwningPawn();

	if (bIsValidBlock)
	{
		// TODO: 格挡成功
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			ArcaneGameplayTags::Shared_Event_MeleeAttack,
			EventData
			);
	}
	
	
}
