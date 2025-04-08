// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ExecCalc/UMMC_PercentageHeal.h"
#include "ArcaneGameplayTags.h"

UMMC_PercentageHeal::UMMC_PercentageHeal()
{
	RelevantAttributesToCapture.Add(GetHealthCaptureDef().CurrentHealthDef);		//	捕获当前生命值
	RelevantAttributesToCapture.Add(GetHealthCaptureDef().MaxHealthDef);		// 捕获最大生命值
}

float UMMC_PercentageHeal::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();	// 获取源标签
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();	// 获取目标标签

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	float MaxHealth = 0.0f;
	GetCapturedAttributeMagnitude(GetHealthCaptureDef().MaxHealthDef, Spec, EvaluateParams, MaxHealth);	// 获取最大生命值

	// 从GameplayEffect的SetByCaller获取百分比（10 表示 10%）
	float Percentage = Spec.GetSetByCallerMagnitude(ArcaneGameplayTags::Shared_SetByCaller_HealPercentage, true, 0.f);	// 获取百分比

	return Percentage * MaxHealth * 0.01f;	// 计算治疗量
}
