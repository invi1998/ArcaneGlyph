// INVI_1998 All Rights Reserved.

#pragma once
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"

#include "ArcaneStructTypes.generated.h"

class UArcaneHeroGameplayAbility;
class UArcaneGameplayAbility;
class UInputMappingContext;
class UArcaneHeroLinkedAnimLayer;

USTRUCT(BlueprintType)
struct FArcaneHeroAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InputTag")
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UArcaneHeroGameplayAbility> AbilityToGrantClass;

	bool IsValid() const;
};


USTRUCT(BlueprintType)
struct FArcaneHeroWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	TSubclassOf<UArcaneHeroLinkedAnimLayer> WeaponAnimLayerToLink;		// 链接的动画层

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (TitleProperty = "InputTag"))
	TArray<FArcaneHeroAbilitySet> DefaultWeaponAbilities;		// 武器能力集合

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	UInputMappingContext* WeaponInputMappingContext;		// 武器输入映射上下文

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FScalableFloat WeaponBaseDamage;		// 武器基础伤害
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	TSoftObjectPtr<UTexture2D> WeaponIcon;		// 武器图标
	
};
