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
	UFUNCTION(BlueprintPure, Category = "Arcane | Ability")
	AArcaneEnemyCharacter* GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Arcane | Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Arcane | Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> InGameplayEffectClass, const FScalableFloat& InBaseDamageValue);

private:
	TWeakObjectPtr<AArcaneEnemyCharacter> CachedArcaneEnemyCharacter;
	
};
