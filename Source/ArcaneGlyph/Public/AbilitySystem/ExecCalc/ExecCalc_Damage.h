// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/ArcaneAttributeSet.h"
#include "ExecCalc_Damage.generated.h"

struct ArcaneDamageCaptureStatics
{
	// 该宏用于声明一个捕获属性的结构体，其中包含了捕获属性的指针，以及捕获属性的标签
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);

	ArcaneDamageCaptureStatics()
	{
		// 通过捕获属性的标签来捕获属性
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, AttackPower, Source, false);	// 捕获源目标的攻击力，不拍快照
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, DefensePower, Target, false);	// 捕获目标的防御力，不拍快照
	}
};

// 该静态函数将在每次调用时返回一个ArcaneDamageStatics的静态实例，以确保我们不会在每次调用时创建一个新的实例
static const ArcaneDamageCaptureStatics& GetArcaneDamageCaptureStatics()
{
	static ArcaneDamageCaptureStatics DStatics;
	return DStatics;
}

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_Damage();

	// 重写函数，用于计算伤害
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
