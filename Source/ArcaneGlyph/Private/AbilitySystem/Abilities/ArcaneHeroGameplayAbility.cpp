// INVI_1998 All Rights Reserved.


#include "AbilitySystem/Abilities/ArcaneHeroGameplayAbility.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneDebugHelper.h"
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

FGameplayEffectSpecHandle UArcaneHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> InGameplayEffectClass, FGameplayTag InCurrentAttackType, float InBaseDamageValue, int32 InUsedComboCount, float InPreEnergyCost)
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

	// 设置能量消耗数值（以键值对的形式设置）
	DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(ArcaneGameplayTags::Player_SetByCaller_PreEnergy, InPreEnergyCost);

	// 设置当前攻击类型和连击数
	if (InCurrentAttackType.IsValid())
	{
		DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackType, InUsedComboCount);
	}
	
	return DamageEffectSpecHandle;
}

FGameplayEffectSpecHandle UArcaneHeroGameplayAbility::MakeHeroRageGainEffectSpecHandle(TSubclassOf<UGameplayEffect> InGameplayEffectClass, FGameplayTag InCurrentAttackType, float InBaseRageGain, int32 InUsedComboCount)
{
	check(InGameplayEffectClass);

	UArcaneAbilitySystemComponent* ASC = GetArcaneAbilitySystemComponentFromActorInfo();
	if (!ASC) return FGameplayEffectSpecHandle();

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(GetAvatarActorFromActorInfo());
	EffectContext.SetAbility(this);
	// 设置 incigator 和 effect causer。Instigator 是拥有生成此技能的能力的人，EffectCauser 是作为效果（如武器）的物理源的 actor。它们可以是相同的。
	EffectContext.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
	
	
	FGameplayEffectSpecHandle RageGainEffectSpecHandle = ASC->MakeOutgoingSpec(
		InGameplayEffectClass,
		GetAbilityLevel(),
		EffectContext
		);

	// 设置怒气基础回复数值
	RageGainEffectSpecHandle.Data->SetSetByCallerMagnitude(
		ArcaneGameplayTags::Shared_SetByCaller_BaseRageGain,
		InBaseRageGain
	);

	// 设置当前攻击类型和连击数
	if (InCurrentAttackType.IsValid())
	{
		RageGainEffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackType, InUsedComboCount);
	}

	return RageGainEffectSpecHandle;
	
}

bool UArcaneHeroGameplayAbility::GetAbilityRemainingCooldownByTag(FGameplayTag InCooldownTag, float& TotalCooldownTime, float& OutRemainingCooldown)
{
	check(InCooldownTag.IsValid());
	
	FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());

	TArray<TPair<float, float>> CooldownList = GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

	if (CooldownList.Num() > 0)
	{
		TotalCooldownTime = CooldownList[0].Value;
		OutRemainingCooldown = CooldownList[0].Key;
		
	}
	else
	{
		TotalCooldownTime = 0;
		OutRemainingCooldown = 0;
	}
	
	return OutRemainingCooldown > 0.f;

}

