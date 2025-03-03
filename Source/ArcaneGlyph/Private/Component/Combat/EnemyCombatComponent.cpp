// INVI_1998 All Rights Reserved.


#include "Component/Combat/EnemyCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneDebugHelper.h"
#include "ArcaneGameplayTags.h"
#include "AbilitySystem/Abilities/ArcaneEnemyGameplayAbility.h"
#include "Characters/ArcaneEnemyCharacter.h"
#include "Components/BoxComponent.h"


void UEnemyCombatComponent::OnHitTargetActor(AActor* InHitActor, int32 InCollisionBoxIndex)
{
	if (!InHitActor) return;

	/*
	if (HitOverlappedActors.Contains(InHitActor))
	{
		// 说明已经击中过了
		return;
	}

	HitOverlappedActors.AddUnique(InHitActor);
	*/
	HitOverlappedActors.Add(InHitActor);

	bool bIsValidBlock = false;
	
	const bool bIsPlayerBlocking = UArcaneBlueprintFunctionLibrary::NativeDoesActorHasGameplayTag(InHitActor, ArcaneGameplayTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable = UArcaneBlueprintFunctionLibrary::NativeDoesActorHasGameplayTag(GetOwningPawn(), ArcaneGameplayTags::Enemy_Status_Unblockable);

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
		if (InCollisionBoxIndex == 1)
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				GetOwningPawn(),
				ArcaneGameplayTags::Shared_Event_MeleeAttack_1,
				EventData
			);
		}
		else
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				GetOwningPawn(),
				ArcaneGameplayTags::Shared_Event_MeleeAttack_2,
				EventData
			);
		}
	}
	
	
}

void UEnemyCombatComponent::ToggleBodyCollisionBoxCollision(bool bEnable, EToggleDamageType InToggleDamageType)
{
	AArcaneEnemyCharacter* EnemyCharacter = Cast<AArcaneEnemyCharacter>(GetOwningPawn());
	if (!EnemyCharacter) return;

	if (InToggleDamageType == EToggleDamageType::LeftHand)
	{
		EnemyCharacter->GetLeftHandCollisionBox()->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
	else if (InToggleDamageType == EToggleDamageType::RightHand)
	{
		EnemyCharacter->GetRightHandCollisionBox()->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
	else if (InToggleDamageType == EToggleDamageType::Head)
	{
		EnemyCharacter->GetHeadCollisionBox()->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}
