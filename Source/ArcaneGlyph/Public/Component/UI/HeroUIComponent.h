// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PawnUIComponent.h"
#include "GameplayTagContainer.h"
#include "HeroUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChangedDelegate, TSoftObjectPtr<UTexture2D>, InEquippedWeaponIcon);	// 装备武器图标变化委托

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityIconSlotUpdatedDelegate, FGameplayTag, AbilityInputTag, TSoftObjectPtr<UTexture2D>, InAbilityIcon);	// 能力图标槽更新委托

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAbilityCooldownBeginDelegate, FGameplayTag, AbilityInputTag, float, InCooldownDuration, float, InCooldownTimeRemaining);	// 能力冷却开始委托

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPotionNumberChangedDelegate, int32, InPotionNumber, int32, InMaxPotionNumber);	// 药水数量变化委托

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPercentChangedWithSparkDelegate, float, NewPercent, int32, InCurrentSpark, int32, InMaxSpark);	// 百分比变化委托

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComboTypeChangedDelegate, FGameplayTag, InComboTypeTag, TSoftObjectPtr<UTexture2D>, InComboTypeIcon);	// 连击类型变化委托

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentUsedItemTagChangedDelegate, FGameplayTag, InCurrentUsedItemTag);	// 当前使用物品标签变化委托

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnegyChangedDelegate, float, InCurrentEnergyPercent);			// 气力变化委托


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARCANEGLYPH_API UHeroUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void BroadcastInitialValues() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedWithSparkDelegate OnCurrentRageChanged;	// 当前怒气值变化

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnEquippedWeaponChangedDelegate OnEquippedWeaponChanged;	// 装备武器变化

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityIconSlotUpdatedDelegate OnAbilityIconSlotUpdated;	// 能力图标槽更新

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityCooldownBeginDelegate OnAbilityCooldownBegin;	// 能力冷却开始

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPotionNumberChangedDelegate OnCurrentHealthPotionChanged;	// 当前生命药水数量变化

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPotionNumberChangedDelegate OnCurrentRagePotionChanged;	// 当前愤怒药水数量变化

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPotionNumberChangedDelegate OnCurrentSparkNumChanged;	// 当前火花数量变化

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnComboTypeChangedDelegate OnComboTypeChanged;				// 连击类型变化

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnEnegyChangedDelegate OnCurrentEnergyChanged;				// 气力变化

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnEnegyChangedDelegate OnCurrentManaChanged;				// 法力值变化

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPotionNumberChangedDelegate OnCurrentManaPotionChanged;	// 当前法力药水数量变化

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPotionNumberChangedDelegate OnCurrenExtraDamagePotionChanged;	// 当前额外伤害药水数量变化

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnCurrentUsedItemTagChangedDelegate OnCurrentUsedItemTagChanged;	// 当前使用物品标签变化
};
