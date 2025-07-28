// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ExecCalc/ExecCale_ManaCost.h"

UExecCale_ManaCost::UExecCale_ManaCost()
{
	RelevantAttributesToCapture.Add(GetArcaneCostManaCaptureStatics().CurrentManaDef);
}

void UExecCale_ManaCost::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float CurrentMana = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetArcaneCostManaCaptureStatics().CurrentManaDef,
		EvaluationParameters,
		CurrentMana
	);

	float ManaCost = 0.f;
	for (const TPair<FGameplayTag, float>& TagMagnitudes : Spec.SetByCallerTagMagnitudes)
	{
		// 如果当前的 SetByCallerTagMagnitudes 中有以 Player.Ability 开头的标签，就取其value值作为Mana的消耗
		FGameplayTagContainer ParentTags;
		ParentTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Ability"));
		if (TagMagnitudes.Key.MatchesAny(ParentTags))
		{
			ManaCost = TagMagnitudes.Value;
			break;
		}
	}
	if (ManaCost < 0.f && FMath::Abs(ManaCost) > CurrentMana)
	{
		ManaCost = -CurrentMana; // 如果消耗的法力值大于当前法力值，则将消耗的法力值设置为当前法力值的负值
	}
	// 扣除法力值
	if (ManaCost != 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetArcaneCostManaCaptureStatics().CurrentManaProperty,
				EGameplayModOp::AddBase,
				ManaCost
			)
		);
	}
}
