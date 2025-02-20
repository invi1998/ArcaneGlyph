// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneGameplayAbility.h"
#include "ArcaneHeroGameplayAbility.generated.h"

class UHeroCombatComponent;
class AArcaneHeroController;
class AArcaneHeroCharacter;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneHeroGameplayAbility : public UArcaneGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Arcane | Ability")
	AArcaneHeroCharacter* GetHeroCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Arcane | Ability")
	AArcaneHeroController* GetHeroControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Arcane | Ability")
	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Arcane | Ability")
	FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> InGameplayEffectClass, FGameplayTag InCurrentAttackType, float InBaseDamageValue, int32 InUsedComboCount);

private:
	TWeakObjectPtr<AArcaneHeroCharacter> CachedArcaneHeroCharacter;		// 缓存的英雄角色
	TWeakObjectPtr<AArcaneHeroController> CachedArcaneHeroController;	// 缓存的英雄控制器
};
