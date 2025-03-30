// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneHeroGameplayAbility.h"
#include "HeroGameplayAbility_PickUpSoul.generated.h"

class AArcanePickUpBase;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UHeroGameplayAbility_PickUpSoul : public UArcaneHeroGameplayAbility
{
	GENERATED_BODY()

protected:
	// Begin UGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~End UGameplayAbility interface

	UFUNCTION(BlueprintCallable)
	void CollectSoul();

private:
	UPROPERTY(EditDefaultsOnly)
	float BoxTraceDistance = 100.f;

	UPROPERTY(EditDefaultsOnly)
	FVector TraceBoxSize = FVector(200.f, 200.f, 200.f);

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> SoulTraceChannels;

	UPROPERTY(EditDefaultsOnly)
	bool bDrawDebug = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<AArcanePickUpBase*> CollectedSouls;
	
};
