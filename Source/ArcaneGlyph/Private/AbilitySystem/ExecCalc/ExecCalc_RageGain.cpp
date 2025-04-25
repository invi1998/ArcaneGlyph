// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ExecCalc/ExecCalc_RageGain.h"

#include "ArcaneDebugHelper.h"
#include "ArcaneGameplayTags.h"

UExecCalc_RageGain::UExecCalc_RageGain()
{
	RelevantAttributesToCapture.Add(GetArcaneRageGainExecCaptureStatics().CurrentSparkDef);	// 当前火花
	RelevantAttributesToCapture.Add(GetArcaneRageGainExecCaptureStatics().CurrentRageDef);	// 当前怒气值
}

void UExecCalc_RageGain::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float CurrentSpark = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetArcaneRageGainExecCaptureStatics().CurrentSparkDef,
		EvaluationParameters,
		CurrentSpark);

	// 从GameplayEffect的SetByCaller获取怒气回复基础值
	float RageGainBase = 0.f;
		
	// 遍历伤害类型
	int32 LightComboCount = 0;
	int32 HeavyComboCount = 0;
	for (const TPair<FGameplayTag, float>& TagMagnitudes : Spec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitudes.Key.MatchesTagExact(ArcaneGameplayTags::Shared_SetByCaller_BaseRageGain))
		{
			RageGainBase = TagMagnitudes.Value;
		}
		
		// 轻击连击次数
		if (TagMagnitudes.Key.MatchesTagExact(ArcaneGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			LightComboCount = TagMagnitudes.Value;
		}

		if (TagMagnitudes.Key.MatchesTagExact(ArcaneGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			HeavyComboCount = TagMagnitudes.Value;
		}
	}

	float RageGainTotal = 0.f;

	if (LightComboCount != 0)
	{
		RageGainTotal += RageGainBase * (CurrentSpark + LightComboCount) + 5.f;	// 轻击连击次数
	}
	if (HeavyComboCount != 0)
	{
		RageGainTotal += RageGainBase* ((CurrentSpark + HeavyComboCount) * 1.25) + 10.f;	// 重击连击次数
	}

	// 伤害向上取整，保留小数点后两位
	RageGainTotal = FMath::CeilToFloat(RageGainTotal * 100.f) / 100.f;

	OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetArcaneRageGainExecCaptureStatics().CurrentRageProperty,
				EGameplayModOp::AddBase,
				RageGainTotal
			)
		);
	
	
}
