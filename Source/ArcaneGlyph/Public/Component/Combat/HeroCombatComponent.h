// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PawnCombatComponent.h"
#include "ScalableFloat.h"
#include "HeroCombatComponent.generated.h"


class AArcaneHeroCharacter;
class AArcaneHeroWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARCANEGLYPH_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	FScalableFloat RageGainBase;	// 愤怒增益基础值

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(GameplayTagFilter="Player.ComboType"))
	FGameplayTag CurrentComboTypeTag;	// 当前连击组合类型标签（当前攻击模组）
	
	FTimerHandle EnergyRegenTimerHandle;	// 气力回复计时器句柄

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	float EnergyRegenDelay = 0.75f;		// 气力回复延迟时间

	UFUNCTION(BlueprintCallable, Category="Components")
	void StartEnergyRegenTimer();

	UFUNCTION(BlueprintCallable, Category="Components")
	void ClearEnergyRegenTimer();

	UFUNCTION(BlueprintCallable, Category="Components")
	void SetEnergyRegenCooldownTag();

	void EnergyRegen();

	UFUNCTION(BlueprintCallable, Category="Components")
	void ChangeCurrentComboTypeTag(AArcaneHeroCharacter* InHeroCharacter, const FGameplayTag& InComboTypeTag);
	
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
