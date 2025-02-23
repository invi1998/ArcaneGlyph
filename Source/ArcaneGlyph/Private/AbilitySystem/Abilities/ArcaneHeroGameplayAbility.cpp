// INVI_1998 All Rights Reserved.


#include "AbilitySystem/Abilities/ArcaneHeroGameplayAbility.h"

#include "ArcaneGameplayTags.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "Characters/ArcaneHeroCharacter.h"
#include "ArcaneGlyph/Public/Controllers/ArcaneHeroController.h"

AArcaneHeroCharacter* UArcaneHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if (!CachedArcaneHeroCharacter.IsValid())
	{
		CachedArcaneHeroCharacter = Cast<AArcaneHeroCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedArcaneHeroCharacter.IsValid() ? CachedArcaneHeroCharacter.Get() : nullptr;
}

AArcaneHeroController* UArcaneHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	if (!CachedArcaneHeroController.IsValid())
	{
		CachedArcaneHeroController = Cast<AArcaneHeroController>(CurrentActorInfo->PlayerController);
	}
	return CachedArcaneHeroController.IsValid() ? CachedArcaneHeroController.Get() : nullptr;
}

UHeroCombatComponent* UArcaneHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	if (AArcaneHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo())
	{
		return HeroCharacter->GetHeroCombatComponent();
	}
	return nullptr;
}

FGameplayEffectSpecHandle UArcaneHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> InGameplayEffectClass, FGameplayTag InCurrentAttackType, float InBaseDamageValue, int32 InUsedComboCount)
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
		InBaseDamageValue
	);

	// 设置当前攻击类型和连击数
	if (InCurrentAttackType.IsValid())
	{
		DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackType, InUsedComboCount);
	}
	
	return DamageEffectSpecHandle;
}
