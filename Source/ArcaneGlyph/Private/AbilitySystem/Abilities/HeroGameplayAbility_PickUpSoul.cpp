// INVI_1998 All Rights Reserved.


#include "AbilitySystem/Abilities/HeroGameplayAbility_PickUpSoul.h"

#include "Characters/ArcaneHeroCharacter.h"
#include "Items/PickUp/ArcanePickUpBase.h"
#include "Kismet/KismetSystemLibrary.h"

void UHeroGameplayAbility_PickUpSoul::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}

void UHeroGameplayAbility_PickUpSoul::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_PickUpSoul::CollectSoul()
{
	CollectedSouls.Empty();
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetHeroCharacterFromActorInfo(),
		GetHeroCharacterFromActorInfo()->GetActorLocation(),
		GetHeroCharacterFromActorInfo()->GetActorLocation() + (-GetHeroCharacterFromActorInfo()->GetActorUpVector() * BoxTraceDistance),
		TraceBoxSize * 0.5f,
		(-GetHeroCharacterFromActorInfo()->GetActorUpVector()).ToOrientationRotator(),
		SoulTraceChannels,
		false,
		TArray<AActor*>(),
		bDrawDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		HitResults,
		true
	);

	for (const FHitResult& HitResult : HitResults)
	{
		if (AArcanePickUpBase* PickUpObject = Cast<AArcanePickUpBase>(HitResult.GetActor()))
		{
			CollectedSouls.AddUnique(PickUpObject);
		}
	}

	if (CollectedSouls.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}

void UHeroGameplayAbility_PickUpSoul::ConsumeSouls()
{
	if (CollectedSouls.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
		return;
	}
	for (AArcanePickUpBase* PickUpObject : CollectedSouls)
	{
		if (PickUpObject)
		{
			PickUpObject->Consume(GetArcaneAbilitySystemComponentFromActorInfo(), GetAbilityLevel());
		}
	}
}
