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

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

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

	UPROPERTY(BlueprintReadOnly, Category = "Spark")
	FGameplayAttributeData RageBaseIncrement;		// 怒气基础增量
	ATTRIBUTE_ACCESSORS(UArcaneAttributeSet, RageBaseIncrement);

private:
	TWeakInterfacePtr<IPawnUIInterface> CachedPawnUIInterface;
	
};
