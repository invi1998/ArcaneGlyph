// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ExecCalc/MMC_RageGainCalculation.h"

#include "ArcaneDebugHelper.h"
#include "ArcaneGameplayTags.h"

UMMC_RageGainCalculation::UMMC_RageGainCalculation()
{
	RelevantAttributesToCapture.Add(GetArcaneRageGainCaptureStatics().CurrentSparkDef);
}

float UMMC_RageGainCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();	// 获取源标签
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();	// 获取目标标签

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;
	
	// 获得当前火花倍数的怒气增益
	float CurrentSpark = 0.f;
	GetCapturedAttributeMagnitude(GetArcaneRageGainCaptureStatics().CurrentSparkDef, Spec, EvaluateParams, CurrentSpark);

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
		RageGainTotal += LightComboCount * RageGainBase * ((CurrentSpark + 1) * 0.5);	// 轻击连击次数
	}
	if (HeavyComboCount != 0)
	{
		RageGainTotal += HeavyComboCount * RageGainBase* ((CurrentSpark + 1) * 1.5);	// 重击连击次数
	}
	
	return RageGainTotal;
}
