// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneWeaponBase.h"
#include "GameplayAbilitySpecHandle.h"
#include "ArcaneTypes/ArcaneStructTypes.h"
#include "Interfaces/ArcaneStatusInterface.h"
#include "ArcaneHeroWeapon.generated.h"

UCLASS()
class ARCANEGLYPH_API AArcaneHeroWeapon : public AArcaneWeaponBase, public IArcaneStatusInterface
{
	GENERATED_BODY()

public:
	AArcaneHeroWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FArcaneHeroWeaponData HeroWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintCallable)
	void AssignGrantedComboGroupAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InGrantedComboGroupAbilitySpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle>& GetGrantedAbilitySpecHandles() { return GrantedAbilitySpecHandles; }

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle>& GetGrantedComboGroupAbilitySpecHandles() { return GrantedComboGroupAbilitySpecHandles; }
	
private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;	// 赋予的能力规格句柄
	TArray<FGameplayAbilitySpecHandle> GrantedComboGroupAbilitySpecHandles;	// 赋予的连招组能力规格句柄

};
