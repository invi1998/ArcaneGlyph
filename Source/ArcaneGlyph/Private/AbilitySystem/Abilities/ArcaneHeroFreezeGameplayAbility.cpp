// INVI_1998 All Rights Reserved.


#include "AbilitySystem/Abilities/ArcaneHeroFreezeGameplayAbility.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneGameplayTags.h"
#include "AbilitySystem/ArcaneAttributeSet.h"

void UArcaneHeroFreezeGameplayAbility::ApplyFreezeEffect(AActor* TargetActor)
{
	FreezeTargetActor = TargetActor;
	UArcaneAbilitySystemComponent* TargetASC = UArcaneBlueprintFunctionLibrary::NativeGetArcaneASCFromActor(TargetActor);
	if (!TargetASC)
	{
		return;
	}

	// 获取目标韧性值
	const UArcaneAttributeSet* TargetAttributeSet = TargetASC->GetSet<UArcaneAttributeSet>();
	if (!TargetAttributeSet) return;

	const float Resilience = TargetAttributeSet->GetResilience();

	// 计算实际定身时长
	float ActualFreezeDuration = FMath::Max(0.1f, BaseRootDuration - (Resilience * ResilienceFactor));

	// 应用定身效果
	FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
	EffectContext.AddSourceObject(GetAvatarActorFromActorInfo());
	
	FGameplayEffectSpecHandle FreezeEffectSpecHandle = TargetASC->MakeOutgoingSpec(FreezeEffectClass, GetAbilityLevel(), EffectContext);

	if (FreezeEffectSpecHandle.IsValid())
	{
		ActiveFreezeEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*FreezeEffectSpecHandle.Data.Get());
	}

	// 设置定身效果的持续时间
	GetWorld()->GetTimerManager().SetTimer(
		FreezeTimerHandle,
		this,
		&UArcaneHeroFreezeGameplayAbility::EndFreezeEffect,
		ActualFreezeDuration,
		false // 不循环
	);

	// 监听伤害事件
	FGameplayTagContainer DamageEventTags;
	DamageEventTags.AddTag(ArcaneGameplayTags::Shared_Event_HitReact);

	DamageEventHandle = TargetASC->AddGameplayEventTagContainerDelegate(
		DamageEventTags,
		FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UArcaneHeroFreezeGameplayAbility::OnDamageReceived)
	);
	
}

void UArcaneHeroFreezeGameplayAbility::OnDamageReceived(FGameplayTag DamageTag, const FGameplayEventData* Payload)
{
	// 确保是当前目标受到伤害
	if (Payload->Target != GetAvatarActorFromActorInfo()) return;

	// 获取当前剩余定身时间
	float RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(FreezeTimerHandle);

	// 减少定身时间（至少保留 0.1秒）
	float NewRemainingTime = FMath::Max(0.1f, RemainingTime - DamageReductionPerHit);

	// 更新定身效果的持续时间
	GetWorld()->GetTimerManager().SetTimer(
		FreezeTimerHandle,
		this,
		&UArcaneHeroFreezeGameplayAbility::EndFreezeEffect,
		NewRemainingTime,
		false // 不循环
	);
}

void UArcaneHeroFreezeGameplayAbility::EndFreezeEffect()
{
	UArcaneAbilitySystemComponent* ASC = UArcaneBlueprintFunctionLibrary::NativeGetArcaneASCFromActor(FreezeTargetActor);
	if (!ASC) return;
	
	// 移除定身效果
	if (ActiveFreezeEffectHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(ActiveFreezeEffectHandle);
	}

	// 清理定时器和事件监听
	if (DamageEventHandle.IsValid())
	{
		FGameplayTagContainer DamageEventTags;
		DamageEventTags.AddTag(ArcaneGameplayTags::Shared_Event_HitReact);
		ASC->RemoveGameplayEventTagContainerDelegate(
			DamageEventTags,
			DamageEventHandle
		);
	}
}
