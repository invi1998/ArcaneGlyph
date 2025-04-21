// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ExecCalc/ExecCalc_MaxRage.h"

UExecCalc_MaxRage::UExecCalc_MaxRage()
{
	RelevantAttributesToCapture.Add(GetArcaneRageCaptureStatics().RageBaseIncrementDef);	// 怒气基础增量
	RelevantAttributesToCapture.Add(GetArcaneRageCaptureStatics().ExtraRageIncrementDef);	// 怒气基础增量
	RelevantAttributesToCapture.Add(GetArcaneRageCaptureStatics().MaxSparkDef);				// 最大火花
	RelevantAttributesToCapture.Add(GetArcaneRageCaptureStatics().MaxRageDef);			// 最大怒气值
}

void UExecCalc_MaxRage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// 获取AbilitySystem组件
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// 捕获输入参数
	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    
	float BaseIncrement = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetArcaneRageCaptureStatics().RageBaseIncrementDef,
		EvaluationParams,
		BaseIncrement
	);

	float ExtraIncrement = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetArcaneRageCaptureStatics().ExtraRageIncrementDef,
		EvaluationParams,
		ExtraIncrement
	);

	float MaxBeans = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetArcaneRageCaptureStatics().MaxSparkDef,
		EvaluationParams,
		MaxBeans
	);

	// 计算最大值
	float CalculatedMax = CalculateLinearMaxRage(BaseIncrement, ExtraIncrement, MaxBeans);

	// 输出到MaxMomentum属性
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetArcaneRageCaptureStatics().MaxRageProperty,
			EGameplayModOp::Override,
			CalculatedMax
		)
	);
}
