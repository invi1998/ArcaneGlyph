// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ArcaneTypes/ArcaneStructTypes.h"
#include "ArcaneAbilitySystemComponent.generated.h"

class AArcaneHeroWeapon;
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

	// UPARAM(ref) 旨在告诉蓝图调用者，这个参数是一个输入参数，但是它是一个引用参数，这意味着它可以被修改, 但是不会被返回
	UFUNCTION(BlueprintCallable, Category = "Arcane | AbilitySystem")
	void RemoveGrantHeroWeaponAbilities(AArcaneHeroWeapon* InWeapon);

	UFUNCTION(BlueprintCallable, Category = "Arcane | AbilitySystem")
	bool TryActivateAbilityByTag(FGameplayTag InAbilityTag);
	
};
