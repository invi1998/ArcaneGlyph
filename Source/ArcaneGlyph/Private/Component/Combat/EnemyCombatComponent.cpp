// INVI_1998 All Rights Reserved.


#include "Component/Combat/EnemyCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArcaneBlueprintFunctionLibrary.h"
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

	bool bIsValidBlock = false;
	
	const bool bIsPlayerBlocking = UArcaneBlueprintFunctionLibrary::NativeDoesActorHasGameplayTag(InHitActor, ArcaneGameplayTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		bIsValidBlock = UArcaneBlueprintFunctionLibrary::IsCurrentBlockValid(GetOwningPawn(), InHitActor);
	}

	FGameplayEventData EventData;
	EventData.Target = InHitActor;
	EventData.Instigator = GetOwningPawn();

	if (bIsValidBlock)
	{
		// 格挡成功，告知格挡者
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			InHitActor,
			ArcaneGameplayTags::Player_Event_BlockSuccess,
			EventData
			);
	}
	else
	{
		// 未被格挡，告知攻击者
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			ArcaneGameplayTags::Shared_Event_MeleeAttack,
			EventData
			);
	}
	
	
}
