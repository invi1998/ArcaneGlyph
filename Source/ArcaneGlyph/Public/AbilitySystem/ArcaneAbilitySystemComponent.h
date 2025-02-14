// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ArcaneTypes/ArcaneStructTypes.h"
#include "ArcaneAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	UFUNCTION(BlueprintCallable, Category = "Arcane | AbilitySystem", meta = (InApplyLevel = "1"))
	void GrantHeroWeaponAbilities(const TArray<FArcaneHeroAbilitySet>& InAbilitySets, int32 InApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);
	
};
