// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "ArcaneGameplayTags.h"

UExecCalc_Damage::UExecCalc_Damage()
{
	// 捕获到伤害属性
	// 有两种方式可以捕获属性
	// 1: 较慢的方式，通过属性名称来查找属性
	/*
	FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
		UArcaneAttributeSet::StaticClass(),
		GET_MEMBER_NAME_CHECKED(UArcaneAttributeSet, AttackPower)
	);
	FGameplayEffectAttributeCaptureDefinition AttackPowerDef(
		AttackPowerProperty,
		EGameplayEffectAttributeCaptureSource::Source,
		false
	);
	RelevantAttributesToCapture.Add(AttackPowerDef);
	*/

	// 2: 更快的方式，通过属性标签来查找属性
	RelevantAttributesToCapture.Add(GetArcaneDamageCaptureStatics().AttackPowerDef);	// 捕获攻击力属性
	RelevantAttributesToCapture.Add(GetArcaneDamageCaptureStatics().DefensePowerDef);	// 捕获防御力属性
	RelevantAttributesToCapture.Add(GetArcaneDamageCaptureStatics().DamageTakenDef);	// 捕获伤害承受属性
	
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	/*
	 * 这些都是我们可以从Spec中获取到的一些信息，是我们在创建GameplayEffectSpec时传入的一些参数
	Spec.GetContext().GetSourceObject();
	Spec.GetContext().GetAbility();
	Spec.GetContext().GetEffectCauser();
	Spec.GetContext().GetInstigator();
	*/
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetArcaneDamageCaptureStatics().AttackPowerDef,
		EvaluationParameters,
		SourceAttackPower);

	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetArcaneDamageCaptureStatics().DefensePowerDef,
		EvaluationParameters,
		TargetDefensePower);

	float BaseDamage = 0.f;
	int32 LightComboCount = 0;
	int32 HeavyComboCount = 0;
	for (const TPair<FGameplayTag, float>& TagMagnitudes : Spec.SetByCallerTagMagnitudes)
	{
		// 这里可以获取到我们在创建GameplayEffectSpec时传入的动态标签
		// 武器基础伤害
		if (TagMagnitudes.Key.MatchesTagExact(ArcaneGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitudes.Value;
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

	if (LightComboCount != 0)
	{
		const float DamageIncreasePercentLight = (LightComboCount-1) * 0.05f + 1.f;
		BaseDamage += BaseDamage * DamageIncreasePercentLight;
	}
	if (HeavyComboCount != 0)
	{
		const float DamageIncreasePercentHeavy = HeavyComboCount * 0.55f + 1.f;
		BaseDamage += BaseDamage * DamageIncreasePercentHeavy;
	}

	float FinalDamage = BaseDamage * SourceAttackPower / TargetDefensePower;
	// 伤害向上取整，保留小数点后两位
	FinalDamage = FMath::CeilToFloat(FinalDamage * 100.f) / 100.f;
	
	if (FinalDamage > 0.f)
	{
		// 将最终伤害值应用到伤害承受属性上
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetArcaneDamageCaptureStatics().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamage
			)
		);
	}
	
}
