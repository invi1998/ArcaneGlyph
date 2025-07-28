// INVI_1998 All Rights Reserved.


#include "AbilitySystem/Abilities/ArcaneGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneGameplayTags.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "Component/Combat/PawnCombatComponent.h"

void UArcaneGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UArcaneGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EArcaneAbilityActivationPolicy::OnGiven)
	{
		// 如果激活策略是在给予时激活，同时 ActorInfo 有效且 Spec 未激活，则激活该技能
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UArcaneGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	if (AbilityActivationPolicy == EArcaneAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
	
}

bool UArcaneGameplayAbility::DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	// Define a common lambda to check for blocked tags
	bool bBlocked = false;
	auto CheckForBlocked = [&](const FGameplayTagContainer& ContainerA, const FGameplayTagContainer& ContainerB)
	{
		// Do we not have any tags in common?  Then we're not blocked
		if (ContainerA.IsEmpty() || ContainerB.IsEmpty() || !ContainerA.HasAny(ContainerB))
		{
			return;
		}
 
		if (OptionalRelevantTags)
		{
			// Ensure the global blocking tag is only added once
			if (!bBlocked)
			{
				UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
				const FGameplayTag& BlockedTag = AbilitySystemGlobals.ActivateFailTagsBlockedTag;
				OptionalRelevantTags->AddTag(BlockedTag);
			}
 
			// Now append all the blocking tags
			OptionalRelevantTags->AppendMatchingTags(ContainerA, ContainerB);
		}
 
		bBlocked = true;
	};
 
	// Define a common lambda to check for missing required tags
	bool bMissing = false;
	auto CheckForRequired = [&](const FGameplayTagContainer& TagsToCheck, const FGameplayTagContainer& RequiredTags)
	{
		// Do we have no requirements, or have met all requirements?  Then nothing's missing
		if (RequiredTags.IsEmpty() || TagsToCheck.HasAll(RequiredTags))
		{
			return;
		}
 
		if (OptionalRelevantTags)
		{
			// Ensure the global missing tag is only added once
			if (!bMissing)
			{
				UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
				const FGameplayTag& MissingTag = AbilitySystemGlobals.ActivateFailTagsMissingTag;
				OptionalRelevantTags->AddTag(MissingTag);
			}
 
			FGameplayTagContainer MissingTags = RequiredTags; 
			MissingTags.RemoveTags(TagsToCheck.GetGameplayTagParents());
			OptionalRelevantTags->AppendTags(MissingTags);
		}
 
		bMissing = true;
	};
 
	// Start by checking all of the blocked tags first (so OptionalRelevantTags will contain blocked tags first)
	CheckForBlocked(GetAssetTags(),AbilitySystemComponent.GetBlockedAbilityTags());
	CheckForBlocked(AbilitySystemComponent.GetOwnedGameplayTags(), ActivationBlockedTags);
	if (SourceTags != nullptr)
	{
		CheckForBlocked(*SourceTags, SourceBlockedTags);
	}
	if (TargetTags != nullptr)
	{
		CheckForBlocked(*TargetTags, TargetBlockedTags);
	}
 
	// Now check all required tags
	CheckForRequired(AbilitySystemComponent.GetOwnedGameplayTags(), ActivationRequiredTags);
	if (SourceTags != nullptr)
	{
		CheckForRequired(*SourceTags, SourceRequiredTags);
	}
	if (TargetTags != nullptr)
	{
		CheckForRequired(*TargetTags, TargetRequiredTags);
	}
 
	// We succeeded if there were no blocked tags and no missing required tags	
	return !bBlocked && !bMissing;
}

UPawnCombatComponent* UArcaneGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UArcaneAbilitySystemComponent* UArcaneGameplayAbility::GetArcaneAbilitySystemComponentFromActorInfo() const
{
	return Cast<UArcaneAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UArcaneGameplayAbility::NativeApplyGameplayEffectSpecToTarget(AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle) const
{
	if (UArcaneAbilitySystemComponent* ArcaneASC = GetArcaneAbilitySystemComponentFromActorInfo())
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InTargetActor);
		if (!TargetASC) return FActiveGameplayEffectHandle();
		
		return ArcaneASC->ApplyGameplayEffectSpecToTarget(
			*InSpecHandle.Data.Get(),
			TargetASC
		);
	}
	return FActiveGameplayEffectHandle();
}

FActiveGameplayEffectHandle UArcaneGameplayAbility::BP_ApplyGameplayEffectSpecToTarget(AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EArcaneSuccessType& SuccessType)
{
	FActiveGameplayEffectHandle ActiveHandle = NativeApplyGameplayEffectSpecToTarget(InTargetActor, InSpecHandle);
	// 根据激活句柄是否成功应用来设置成功类型
	SuccessType = ActiveHandle.WasSuccessfullyApplied() ? EArcaneSuccessType::Success : EArcaneSuccessType::Fail;
	return ActiveHandle;
}

void UArcaneGameplayAbility::ApplyGameplayEffectToHitResults(const TArray<FHitResult>& InHitResults, const FGameplayEffectSpecHandle& InSpecHandle)
{
	if (InHitResults.IsEmpty()) return;

	// 对InHitResults去重，因为InHitResults里面可能会包含多次同一个Pawn
	TArray<APawn*> HitPawns;
	HitPawns.Empty();

	APawn* OwningPawn = Cast<APawn>(GetAvatarActorFromActorInfo());

	for (const FHitResult& HitResult : InHitResults)
	{
		if (APawn* HitPawn = Cast<APawn>(HitResult.GetActor()))
		{
			if (HitPawns.Contains(HitPawn)) continue;
			HitPawns.AddUnique(HitPawn);
			
			if (UArcaneBlueprintFunctionLibrary::IsTargetPawnHostile(OwningPawn, HitPawn))
			{
				FActiveGameplayEffectHandle ActiveHandle = NativeApplyGameplayEffectSpecToTarget(HitPawn, InSpecHandle);
				if (ActiveHandle.WasSuccessfullyApplied())
				{
					FGameplayEventData EventData;
					EventData.Instigator = OwningPawn;
					EventData.Target = HitPawn;
					
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						HitPawn,
						ArcaneGameplayTags::Shared_Event_HitReact,
						EventData
					);
				}
			}
		}
	}
}

void UArcaneGameplayAbility::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FName& SocketName, AActor* HomingTarget, bool bOverridePitch, float Pitch)
{
	
}

FGameplayEffectSpecHandle UArcaneGameplayAbility::MakeRageCostOrGainEffectSpecHandle(TSubclassOf<UGameplayEffect> InGameplayEffectClass, bool bIsCost)
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
		ArcaneGameplayTags::Shared_SetByCaller_RageGainOrCost,
		bIsCost ? -1.f : 1.f
	);
	
	return DamageEffectSpecHandle;
}

FGameplayEffectSpecHandle UArcaneGameplayAbility::MakeEnergyCostEffectSpecHandle(TSubclassOf<UGameplayEffect> InGameplayEffectClass, FGameplayTag InAbilityTag, float InEnergyCostValue) const
{
	check(InGameplayEffectClass);

	UArcaneAbilitySystemComponent* ASC = GetArcaneAbilitySystemComponentFromActorInfo();
	if (!ASC) return FGameplayEffectSpecHandle();

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(GetAvatarActorFromActorInfo());
	EffectContext.SetAbility(this);
	// 设置 incigator 和 effect causer。Instigator 是拥有生成此技能的能力的人，EffectCauser 是作为效果（如武器）的物理源的 actor。它们可以是相同的。
	EffectContext.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
	
	FGameplayEffectSpecHandle EnergyCostSpecHandle = ASC->MakeOutgoingSpec(
		InGameplayEffectClass,
		GetAbilityLevel(),
		EffectContext
		);

	// 设置能量消耗数值（以键值对的形式设置）
	EnergyCostSpecHandle.Data->SetSetByCallerMagnitude(
		InAbilityTag,
		InEnergyCostValue
	);
	
	return EnergyCostSpecHandle;
}

FGameplayEffectSpecHandle UArcaneGameplayAbility::MakeManaCostEffectSpecHandle(TSubclassOf<UGameplayEffect> InGameplayEffectClass, FGameplayTag InAbilityTag, float InManaCostValue) const
{
	check(InGameplayEffectClass);
	UArcaneAbilitySystemComponent* ASC = GetArcaneAbilitySystemComponentFromActorInfo();
	if (!ASC) return FGameplayEffectSpecHandle();
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(GetAvatarActorFromActorInfo());
	EffectContext.SetAbility(this);
	// 设置 incigator 和 effect causer。Instigator 是拥有生成此技能的能力
	// 的人，EffectCauser 是作为效果（如武器）的物理源的 actor。它们可以是相同的。
	EffectContext.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
	FGameplayEffectSpecHandle ManaCostSpecHandle = ASC->MakeOutgoingSpec(
		InGameplayEffectClass,
		GetAbilityLevel(),
		EffectContext
		);
	// 设置法力值消耗数值（以键值对的形式设置）
	ManaCostSpecHandle.Data->SetSetByCallerMagnitude(
		InAbilityTag,
		InManaCostValue
	);
	return ManaCostSpecHandle;
}
