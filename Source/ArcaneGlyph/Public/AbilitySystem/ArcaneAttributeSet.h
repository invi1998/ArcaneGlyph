// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "ArcaneAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class UHeroUIComponent;
class IPawnCombatInterface;
class IPawnUIInterface;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UArcaneAttributeSet();

	void BroadcastCharacterAttributeInfo(const UHeroUIComponent* InHeroUIComponent) const;	// 广播角色属性信息

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	float CalculateTierThreshold(int32 Tier) const;		// 计算当前豆子的棍势
	int32 CalculateCurrentSpark() const;
	float GetCurrentSegmentRagePercent() const;		// 当前阶段怒气值百分比

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, CurrentHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Rage")
	FGameplayAttributeData CurrentRage;
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, CurrentRage);

	UPROPERTY(BlueprintReadOnly, Category = "Rage")
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, MaxRage);

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, AttackPower);

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, DefensePower);

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, DamageTaken);		// 伤害承受

	UPROPERTY(BlueprintReadOnly, Category = "Potion")
	FGameplayAttributeData HealthPotion;		//	生命药水
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, HealthPotion);

	UPROPERTY(BlueprintReadOnly, Category = "Potion")
	FGameplayAttributeData MaxHealthPotion;		// 最大生命药水容量
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, MaxHealthPotion);

	UPROPERTY(BlueprintReadOnly, Category = "Potion")
	FGameplayAttributeData RagePotion;		// 怒气药水
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, RagePotion);

	UPROPERTY(BlueprintReadOnly, Category = "Potion")
	FGameplayAttributeData MaxRagePotion;		// 最大怒气药水容量
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, MaxRagePotion);

	UPROPERTY(BlueprintReadOnly, Category = "Spark")
	FGameplayAttributeData MaxSpark;		// 最大火花数
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, MaxSpark);

	UPROPERTY(BlueprintReadOnly, Category = "Spark")
	FGameplayAttributeData CurrentSpark;		// 当前火花数
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, CurrentSpark);

	UPROPERTY(BlueprintReadOnly, Category = "Increment")
	FGameplayAttributeData RageBaseIncrement;		// 怒气基础增量
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, RageBaseIncrement);

	UPROPERTY(BlueprintReadOnly, Category = "Increment")
	FGameplayAttributeData ExtraRageIncrement;		// 额外怒气步长增量
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, ExtraRageIncrement);

	// 气力
	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData CurrentEnergy;
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, CurrentEnergy);

	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, MaxEnergy);

	// 法力
	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	FGameplayAttributeData CurrentMana;
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, CurrentMana);

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, MaxMana);

	// 法力药水
	UPROPERTY(BlueprintReadOnly, Category = "ManaPotion")
	FGameplayAttributeData ManaPotion;		// 法力药水
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, ManaPotion);

	// 最大法力药水容量
	UPROPERTY(BlueprintReadOnly, Category = "ManaPotion")
	FGameplayAttributeData MaxManaPotion;		// 最大法力药水容量
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, MaxManaPotion);

	// 额外增伤
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData ExtraDamage;
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, ExtraDamage);

	// 增伤药
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamagePotion;		// 增伤药
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, DamagePotion);

	// 最大增伤药容量
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData MaxDamagePotion;		// 最大增伤药容量
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, MaxDamagePotion);

	// 韧性
	UPROPERTY(BlueprintReadOnly, Category = "Resilience")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, Resilience);

	// 定身持续时长
	UPROPERTY(BlueprintReadOnly, Category = "Freeze")
	FGameplayAttributeData FreezeDuration;
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, FreezeDuration);

private:
	TWeakInterfacePtr<IPawnUIInterface> CachedPawnUIInterface;
	TWeakInterfacePtr<IPawnCombatInterface> CachedPawnCombatInterface;
};
