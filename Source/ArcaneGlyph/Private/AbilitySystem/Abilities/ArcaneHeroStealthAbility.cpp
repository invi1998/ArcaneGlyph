// INVI_1998 All Rights Reserved.


#include "AbilitySystem/Abilities/ArcaneHeroStealthAbility.h"

#include "ArcaneGameplayTags.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "Characters/PlayerPhantom.h"
#include "Controllers/ArcaneHeroController.h"

FOnMeleeAttackEventDelegate UArcaneHeroStealthAbility::OnMeleeAttack;

void UArcaneHeroStealthAbility::RecoverCharacterTeamID()
{
	if (AArcaneHeroController* HeroController = GetHeroControllerFromActorInfo())
	{
		HeroController->SetGenericTeamId(FGenericTeamId(0)); // 恢复玩家的团队 ID 为 0

		OnMeleeAttack.Broadcast();
	}
}

void UArcaneHeroStealthAbility::OnReceiveMeleeAttackEvent(FGameplayTag GameplayTag, const FGameplayEventData* GameplayEventData)
{
	RecoverCharacterTeamID();
	
	OnReceiveMeleeAttack();

	if (OnMeleeAttackEventDelegateHandle.IsValid())
	{
		// 清理监听
		FGameplayTagContainer MeleeAttackEventTags;
		MeleeAttackEventTags.AddTag(ArcaneGameplayTags::Shared_Event_MeleeAttack_1);
		UArcaneAbilitySystemComponent* AbilitySystemComponent = GetArcaneAbilitySystemComponentFromActorInfo();
		AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(MeleeAttackEventTags, OnMeleeAttackEventDelegateHandle);
		OnMeleeAttackEventDelegateHandle.Reset();
		
	}
}

void UArcaneHeroStealthAbility::ListhenMeleeAttackEventReceived()
{
	UArcaneAbilitySystemComponent* AbilitySystemComponent = GetArcaneAbilitySystemComponentFromActorInfo();
	
	// 监听 Shared_Event_MeleeAttack_1 事件，即当玩家在隐身状态时，如果发动攻击并且攻击命中AI，则结束当前隐身状态
	FGameplayTagContainer MeleeAttackEventTags;
	MeleeAttackEventTags.AddTag(ArcaneGameplayTags::Shared_Event_MeleeAttack_1);

	OnMeleeAttackEventDelegateHandle = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(
		MeleeAttackEventTags,
		FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UArcaneHeroStealthAbility::OnReceiveMeleeAttackEvent)
	);
	
}


