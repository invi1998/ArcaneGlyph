// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneGameplayAbility.h"
#include "ArcaneEnemyGameplayAbility.generated.h"

class UEnemyCombatComponent;
class AArcaneEnemyCharacter;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneEnemyGameplayAbility : public UArcaneGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintPure, Category = "Arcane | Ability")
	AArcaneEnemyCharacter* GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Arcane | Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Arcane | Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> InGameplayEffectClass, const FScalableFloat& InBaseDamageValue);

	UFUNCTION(BlueprintPure, Category = "Arcane | Ability")
	AActor* GetCurrentTargetActorFromAI();
	
private:
	TWeakObjectPtr<AArcaneEnemyCharacter> CachedArcaneEnemyCharacter;
	
};
