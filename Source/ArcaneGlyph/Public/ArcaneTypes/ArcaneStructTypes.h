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

// 方向范围定义（角度单位为度，覆盖-180°~180°环形范围）
USTRUCT(BlueprintType)
struct FArcaneLocomotionDirectionSettings
{
    GENERATED_BODY()

    // 各方向角度范围（Min/Max）
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float FMin = -22.5f;    // Forward起始角度

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float FMax = 22.5f;     // Forward结束角度

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float FRMin = 22.5f;    // ForwardRight起始角度

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float FRMax = 67.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float RMin = 67.5f;     // Right起始角度

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float RMax = 112.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float BRMin = 112.5f;   // BackwardRight起始角度

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float BRMax = 157.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float BMin = 157.5f;    // Backward起始角度（左后）

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float BMax = -157.5f;   // Backward结束角度（右后）

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float BLMin = -157.5f;  // BackwardLeft起始角度

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float BLMax = -112.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float LMin = -112.5f;   // Left起始角度

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float LMax = -67.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float FLMin = -67.5f;   // ForwardLeft起始角度

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float FLMax = -22.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
    float DeadZone = 10.f; // 死区角度（推荐10°~15°）
};

USTRUCT(BlueprintType)
struct FArcaneLocomotionDirectionSettings_4D
{
	GENERATED_BODY()

	// 各方向角度范围（Min/Max）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
	float FMin = -50.f;    // Forward起始角度

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
	float FMax = 50.f;     // Forward结束角度

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
	float BMin = -130.f;    // Backward起始角度（左后）

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
	float BMax = -130.f;   // Backward结束角度（右后）

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Direction")
	float DeadZone = 20.0f; // 死区角度（推荐10°~15°）
};


