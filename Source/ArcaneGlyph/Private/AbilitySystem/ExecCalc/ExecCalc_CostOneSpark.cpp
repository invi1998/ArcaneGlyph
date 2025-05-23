// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ExecCalc/ExecCalc_CostOneSpark.h"

UExecCalc_CostOneSpark::UExecCalc_CostOneSpark()
{
	RelevantAttributesToCapture.Add(GetArcaneCostOneSparkCaptureStatics().CurrentSparkDef);
	RelevantAttributesToCapture.Add(GetArcaneCostOneSparkCaptureStatics().CurrentRageDef);
	RelevantAttributesToCapture.Add(GetArcaneCostOneSparkCaptureStatics().RageBaseIncrementDef);
	RelevantAttributesToCapture.Add(GetArcaneCostOneSparkCaptureStatics().ExtraRageIncrementDef);
	RelevantAttributesToCapture.Add(GetArcaneCostOneSparkCaptureStatics().MaxSparkDef);
}

void UExecCalc_CostOneSpark::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// 消耗一点Spark
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters EvaluationParameters;

	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float CurrentSpark = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetArcaneCostOneSparkCaptureStatics().CurrentSparkDef,
		EvaluationParameters,
		CurrentSpark);

	float CurrentRageDef = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetArcaneCostOneSparkCaptureStatics().CurrentRageDef,
		EvaluationParameters,
		CurrentRageDef);

	float RageBaseIncrement = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetArcaneCostOneSparkCaptureStatics().RageBaseIncrementDef,
		EvaluationParameters,
		RageBaseIncrement);

	float ExtraRageIncrement = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetArcaneCostOneSparkCaptureStatics().ExtraRageIncrementDef,
		EvaluationParameters,
		ExtraRageIncrement);

	float MaxSparkDef = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetArcaneCostOneSparkCaptureStatics().MaxSparkDef,
		EvaluationParameters,
		MaxSparkDef);

	float CostRage = CalculateCurrentSegmentRage(
		CurrentRageDef,
		MaxSparkDef,
		RageBaseIncrement,
		ExtraRageIncrement);

	if (CostRage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			GetArcaneCostOneSparkCaptureStatics().CurrentRageProperty,
			EGameplayModOp::AddBase,
			-CostRage));
	}
	
}

float UExecCalc_CostOneSpark::CalculateTierThreshold(int32 Tier, float InRageBaseIncrement, float InExtraRageIncrement) const
{
	if (Tier <= 0) return 0.f;

	// 公式：Threshold(n) = BaseIncrement * n + ExtraIncrement * (n(n-1)/2)
	return InRageBaseIncrement * Tier + InExtraRageIncrement * (Tier * (Tier - 1) / 2.0f);
}

int32 UExecCalc_CostOneSpark::CalculateCurrentSpark(float InCurrentRage, float InMaxSpark, float InRageBaseIncrement, float InExtraRageIncrement) const
{
	float CurrentValue = InCurrentRage;
	int32 CalculatedSpark = 0;
	for (int32 Tier = 1; Tier <= InMaxSpark; ++Tier)
	{
		float TierThreshold = CalculateTierThreshold(Tier, InRageBaseIncrement, InExtraRageIncrement);
		if (CurrentValue >= TierThreshold)
		{
			CalculatedSpark = Tier;
		}
		else
		{
			break;
		}
	}
	return CalculatedSpark;
}

float UExecCalc_CostOneSpark::CalculateCurrentSegmentRage(float InCurrentRage, float InMaxSpark, float InRageBaseIncrement, float InExtraRageIncrement) const
{
	const int32 CurrentTier = CalculateCurrentSpark(InCurrentRage, InMaxSpark, InRageBaseIncrement, InExtraRageIncrement);
	const float CurrentValue = InCurrentRage;

	// 计算当前段起始和结束阈值
	const float StartThreshold = (CurrentTier == 0) ? 0.0f : CalculateTierThreshold(CurrentTier, InRageBaseIncrement, InExtraRageIncrement);
	const float EndThreshold = CalculateTierThreshold(CurrentTier + 1, InRageBaseIncrement, InExtraRageIncrement);
	
	return EndThreshold - StartThreshold;
}
