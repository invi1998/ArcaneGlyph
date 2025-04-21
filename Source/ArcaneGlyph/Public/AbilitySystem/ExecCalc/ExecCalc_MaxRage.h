// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/ArcaneAttributeSet.h"
#include "ExecCalc_MaxRage.generated.h"

struct ArcaneRageCaptureStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(RageBaseIncrement);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ExtraRageIncrement);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxSpark);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxRage);

	ArcaneRageCaptureStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, RageBaseIncrement, Source, false);	// 捕获怒气基础增量
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, ExtraRageIncrement, Source, false);	// 捕获怒气额外增量
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, MaxSpark, Source, false);	// 捕获最大火花
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, MaxRage, Source, false);	// 捕获最大怒气
	}
};

static const ArcaneRageCaptureStatics& GetArcaneRageCaptureStatics()
{
	static ArcaneRageCaptureStatics ArcaneRageCaptureDef;
	return ArcaneRageCaptureDef;
}

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UExecCalc_MaxRage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_MaxRage();

	// 核心计算逻辑
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	// 线性公式：等差数列求和
	static float CalculateLinearMaxRage(float BaseIncrement, float ExtraIncrement, int32 MaxBeans)
	{
		return BaseIncrement * MaxBeans + ExtraIncrement * (MaxBeans * (MaxBeans - 1) / 2.f);
	}
};
