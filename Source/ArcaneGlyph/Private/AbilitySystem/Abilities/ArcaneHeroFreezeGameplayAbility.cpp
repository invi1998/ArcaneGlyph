// INVI_1998 All Rights Reserved.


#include "AbilitySystem/Abilities/ArcaneHeroFreezeGameplayAbility.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneDebugHelper.h"
#include "ArcaneGameplayTags.h"
#include "AbilitySystem/ArcaneAttributeSet.h"
#include "Controllers/ArcaneAIController.h"

void UArcaneHeroFreezeGameplayAbility::ApplyFreezeEffect(AActor* TargetActor)
{
	FreezeTargetActor = TargetActor;
	UArcaneAbilitySystemComponent* TargetASC = UArcaneBlueprintFunctionLibrary::NativeGetArcaneASCFromActor(TargetActor);
	if (!TargetASC)
	{
		return;
	}
	
	// 设置定身目标的行为树的黑板值
	if (AArcaneAIController* AIController = Cast<AArcaneAIController>(FreezeTargetActor))
	{
		AIController->SetFreezeState(true);
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

	// 监听伤害事件和死亡状态
	FGameplayTagContainer DamageEventTags;
	DamageEventTags.AddTag(ArcaneGameplayTags::Shared_Event_HitReact);

	DamageEventHandle = TargetASC->AddGameplayEventTagContainerDelegate(
		DamageEventTags,
		FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UArcaneHeroFreezeGameplayAbility::OnDamageReceived)
	);
	
	// 监听死亡状态
	TargetASC->OnActorDeathDelegate.AddUObject(this, &UArcaneHeroFreezeGameplayAbility::OnFreezeTargetDeath);
	
}

void UArcaneHeroFreezeGameplayAbility::OnDamageReceived(FGameplayTag DamageTag, const FGameplayEventData* Payload)
{
	// 确保是当前目标受到伤害
	if (Payload->Instigator != GetAvatarActorFromActorInfo()) return;

	// 获取当前剩余定身时间
	float RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(FreezeTimerHandle);

	// 减少定身时间（至少保留 0.1秒）
	float NewRemainingTime = FMath::Max(0.1f, RemainingTime - DamageReductionPerHit);

	// 如果是重击，那么将直接减少定身时间到0.1s，即立即打破定身状态
	if (Payload->EventMagnitude > 0.f) // 0是重击的阈值
	{
		NewRemainingTime = 0.1f;
	}

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

	BP_OnFreezeEffectEnded();
	
	if (AArcaneAIController* AIController = Cast<AArcaneAIController>(FreezeTargetActor))
	{
		AIController->SetFreezeState(false);
	}
	
	UArcaneAbilitySystemComponent* ASC = UArcaneBlueprintFunctionLibrary::NativeGetArcaneASCFromActor(FreezeTargetActor);
	if (!ASC)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
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
		DamageEventTags.AddTag(ArcaneGameplayTags::Shared_Status_Dead);
		ASC->RemoveGameplayEventTagContainerDelegate(
			DamageEventTags,
			DamageEventHandle
		);

		DamageEventHandle.Reset();
	}
	
	ASC->OnActorDeathDelegate.RemoveAll(this);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UArcaneHeroFreezeGameplayAbility::OnFreezeTargetDeath(AActor* Actor)
{
	if (Actor == FreezeTargetActor)
	{
		EndFreezeEffect();
	}
}
