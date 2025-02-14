// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneWeaponBase.h"
#include "GameplayAbilitySpecHandle.h"
#include "ArcaneTypes/ArcaneStructTypes.h"
#include "ArcaneHeroWeapon.generated.h"

UCLASS()
class ARCANEGLYPH_API AArcaneHeroWeapon : public AArcaneWeaponBase
{
	GENERATED_BODY()

public:
	AArcaneHeroWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FArcaneHeroWeaponData HeroWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintPure)
	const TArray<FGameplayAbilitySpecHandle>& GetGrantedAbilitySpecHandles() const { return GrantedAbilitySpecHandles; }

private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;	// 赋予的能力规格句柄

};
