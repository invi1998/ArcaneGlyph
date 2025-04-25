// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PawnCombatComponent.h"
#include "ScalableFloat.h"
#include "HeroCombatComponent.generated.h"


class AArcaneHeroWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARCANEGLYPH_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	FScalableFloat RageGainBase;	// 愤怒增益基础值
	
	UFUNCTION(BlueprintCallable, Category="Arcane|Combat")
	AArcaneHeroWeapon* GetHeroCarriedWeaponByTag(const FGameplayTag& InWeaponTag) const;

	UFUNCTION(BlueprintCallable, Category="Arcane|Combat")
	AArcaneHeroWeapon* GetHeroCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category="Arcane|Combat")
	float GetHeroCurrenEquippedWeaponDamageAtLevel(float InLevel) const;

	UFUNCTION(BlueprintCallable, Category="Arcane|Combat")
	float GetHeroCurrenRageGainBaseAtlevel(float InLevel) const;
	
	virtual void OnHitTargetActor(AActor* InHitActor, int32 InCollisionBoxIndex) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InHitActor, int32 InCollisionBoxIndex) override;
	
};
