// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/ArcaneAttributeSet.h"
#include "ExecCalc_CostOneSpark.generated.h"

struct ArcaneCostOneSparkCaptureStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentSpark);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentRage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(RageBaseIncrement);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ExtraRageIncrement);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxSpark);

	ArcaneCostOneSparkCaptureStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, CurrentSpark, Source, false);	// 捕获怒气基础增量
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, CurrentRage, Source, false);	// 捕获怒气额外增量
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, RageBaseIncrement, Source, false);	// 捕获怒气基础增量
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, ExtraRageIncrement, Source, false);	// 捕获怒气额外增量
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, MaxSpark, Source, false);	// 捕获最大花火数
	}
};

static const ArcaneCostOneSparkCaptureStatics& GetArcaneCostOneSparkCaptureStatics()
{
	static ArcaneCostOneSparkCaptureStatics ArcaneCostOneSparkCaptureDef;
	return ArcaneCostOneSparkCaptureDef;
}

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UExecCalc_CostOneSpark : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_CostOneSpark();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	float CalculateTierThreshold(int32 Tier, float InRageBaseIncrement, float InExtraRageIncrement) const;
	int32 CalculateCurrentSpark(float InCurrentRage, float InMaxSpark, float InRageBaseIncrement, float InExtraRageIncrement) const;
	float CalculateCurrentSegmentRage(float InCurrentRage, float InMaxSpark, float InRageBaseIncrement, float InExtraRageIncrement) const;
	
};
