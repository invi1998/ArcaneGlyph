// INVI_1998 All Rights Reserved.


#include "AbilitySystem/Abilities/ArcaneEnemyGameplayAbility.h"

#include "ArcaneGameplayTags.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "Characters/ArcaneEnemyCharacter.h"

AArcaneEnemyCharacter* UArcaneEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedArcaneEnemyCharacter.IsValid())
	{
		CachedArcaneEnemyCharacter = Cast<AArcaneEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedArcaneEnemyCharacter.IsValid() ? CachedArcaneEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UArcaneEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	if (AArcaneEnemyCharacter* EnemyCharacter = GetEnemyCharacterFromActorInfo())
	{
		return EnemyCharacter->GetEnemyCombatComponent();
	}
	return nullptr;
}

FGameplayEffectSpecHandle UArcaneEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> InGameplayEffectClass,  const FScalableFloat& InBaseDamageValue)
{
	check(InGameplayEffectClass);

	UArcaneAbilitySystemComponent* ASC = GetArcaneAbilitySystemComponentFromActorInfo();
	if (!ASC) return FGameplayEffectSpecHandle();

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(GetAvatarActorFromActorInfo());
	EffectContext.SetAbility(this);
	// 设置 incigator 和 effect causer。Instigator 是拥有生成此技能的能力的人，EffectCauser 是作为效果（如武器）的物理源的 actor。它们可以是相同的。
	EffectContext.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
	
	
	FGameplayEffectSpecHandle DamageEffectSpecHandle = ASC->MakeOutgoingSpec(
		InGameplayEffectClass,
		GetAbilityLevel(),
		EffectContext
		);

	// 设置伤害数值（以键值对的形式设置）
	DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(
		ArcaneGameplayTags::Shared_SetByCaller_BaseDamage,
		InBaseDamageValue.GetValueAtLevel(GetAbilityLevel())
	);
	
	return DamageEffectSpecHandle;
}
