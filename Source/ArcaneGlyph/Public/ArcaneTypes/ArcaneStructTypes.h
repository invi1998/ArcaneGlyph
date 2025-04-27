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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Category = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UArcaneHeroGameplayAbility> AbilityToGrantClass;

	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FArcaneHeroSpecialAbilitySet : public FArcaneHeroAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SoftAbilityIconTexture2D;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Category = "Player.Cooldown"))
	FGameplayTag AbilityCooldownTag;
	
};

USTRUCT(BlueprintType)
struct FArcaneGaitSetting
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxWalkSpeed;		// 最大行走速度

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxAcceleration;	// 最大加速度

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BreakingDeceleration;	// 刹车减速度

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BrakingFrictionFactor;	// 刹车摩擦因子

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bUseSeparateBrakingFriction;	// 是否使用单独的刹车摩擦因子

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BreakFriction;	// 刹车摩擦力

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxJumpHeight;	// 最大跳跃高度
};


USTRUCT(BlueprintType)
struct FArcaneHeroWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	TSubclassOf<UArcaneHeroLinkedAnimLayer> WeaponAnimLayerToLink;		// 链接的动画层

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (TitleProperty = "InputTag"))
	TArray<FArcaneHeroAbilitySet> DefaultWeaponAbilities;		// 武器能力集合

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (TitleProperty = "InputTag"))
	TArray<FArcaneHeroSpecialAbilitySet> SpecialWeaponAbilities;		// 特殊武器能力集合

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	UInputMappingContext* WeaponInputMappingContext;		// 武器输入映射上下文

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FScalableFloat WeaponBaseDamage;		// 武器基础伤害
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	TSoftObjectPtr<UTexture2D> WeaponIcon;		// 武器图标
	
};

USTRUCT(BlueprintType)
struct FArcaneLocomotionDirectionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BMin = -180.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BMax = 180.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BLMin = -157.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BLMax = -112.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LMin = -112.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LMax = -67.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FLMin = -67.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FLMax = -22.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FMin = -22.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FMax = 22.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FRMin = 22.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FRMax = 67.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RMin = 67.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RMax = 112.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BRMin = 112.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BRMax = 157.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DeadZone = 22.5f;	// 死区
};
