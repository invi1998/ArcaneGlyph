// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ExecCalc/ExecCalc_EnergyCost.h"

#include "ArcaneGameplayTags.h"

UExecCalc_EnergyCost::UExecCalc_EnergyCost()
{
	RelevantAttributesToCapture.Add(GetArcaneCostEnergyCaptureStatics().CurrentEnergyDef);
}

void UExecCalc_EnergyCost::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters EvaluationParameters;

	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 获取当前气力
	float CurrentEnergy = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetArcaneCostEnergyCaptureStatics().CurrentEnergyDef,
		EvaluationParameters,
		CurrentEnergy);

	float EnergyCost = 0.f;
	for (const TPair<FGameplayTag, float>& Pair : Spec.SetByCallerTagMagnitudes)
	{
		// 如果当前的 SetByCallerTagMagnitudes 中有以 Player.Ability 开头的标签，就取其value值作为Enegey的消耗
		FGameplayTagContainer ParentTags;
		ParentTags.AddTag(FGameplayTag::RequestGameplayTag("Player.Ability"));
		if (Pair.Key.MatchesAny(ParentTags))
		{
			EnergyCost = Pair.Value;
			break;
		}
	}

	if (EnergyCost < 0.f && FMath::Abs(EnergyCost) > CurrentEnergy)
	{
		// 如果能量消耗小于0，并且大于当前能量，则不进行任何操作
		EnergyCost = -CurrentEnergy; // 将能量消耗设置为当前能量的负值，表示消耗所有剩余能量
	}

	if (EnergyCost != 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			GetArcaneCostEnergyCaptureStatics().CurrentEnergyProperty,
			EGameplayModOp::AddBase,
			EnergyCost));
	}
	
}
