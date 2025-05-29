// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneDebugHelper.h"
#include "ArcaneGameplayTags.h"
#include "Component/Combat/HeroCombatComponent.h"

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
	RelevantAttributesToCapture.Add(GetArcaneDamageCaptureStatics().CurrentSparkDef);	// 捕获当前火花属性
	RelevantAttributesToCapture.Add(GetArcaneDamageCaptureStatics().DamageTakenDef);	// 捕获伤害承受属性
	RelevantAttributesToCapture.Add(GetArcaneDamageCaptureStatics().MaxEnergyDef);		// 捕获最大能量属性
	
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

	float CurrentSpark = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetArcaneDamageCaptureStatics().CurrentSparkDef,
		EvaluationParameters,
		CurrentSpark);

	float BaseDamage = 0.f;
	int32 LightComboCount = 0;
	int32 HeavyComboCount = 0;
	float CurrentEnergy = 0.f;
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

		if (TagMagnitudes.Key.MatchesTagExact(ArcaneGameplayTags::Player_SetByCaller_PreEnergy))
		{
			CurrentEnergy = TagMagnitudes.Value;
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

	if (CurrentSpark > 0.f)
	{
		BaseDamage *= CurrentSpark * 0.5;		// 乘以当前火花值
	}

	// 伤害有个百分之5.5的浮动
	const float DamageRandomPercent = FMath::FRandRange(-0.055f, 0.055f);
	BaseDamage += BaseDamage * DamageRandomPercent;

	float FinalDamage = BaseDamage * SourceAttackPower / TargetDefensePower;
	// 伤害向上取整，保留小数点后两位
	FinalDamage = FMath::CeilToFloat(FinalDamage * 100.f) / 100.f;

	// 捕获当前气力值，在气力充盈（百分之20以上）的情况下，角色的伤害会有额外加成（暴击加成）
	// 在气力耗尽的情况下，角色的伤害将大幅降低（0.25倍）

	float MaxEnergy = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetArcaneDamageCaptureStatics().MaxEnergyDef,
		EvaluationParameters,
		MaxEnergy);

	bool bIsCriticalHit = false;

	// 根据当前气力值调整最终伤害
	if (MaxEnergy > 0.f)
	{
		if (CurrentEnergy > 0.f)
		{
			float CritChance = 0.2f;
			CritChance += (FMath::Max3(LightComboCount, HeavyComboCount, FMath::RoundToInt(CurrentSpark)) / 10.f);
			// 气力充盈，伤害增加50%
			// 计算是否暴击（暴击率为20%）
			bIsCriticalHit = FMath::FRand() < CritChance; // 随机生成一个0到1之间的浮点数，如果小于暴击率，则触发暴击
			if (bIsCriticalHit)
			{
				// 触发暴击
				FinalDamage *= 1.5f; // 暴击伤害翻倍
			}
		}
		else if (CurrentEnergy <= 0.f)
		{
			// 气力耗尽，伤害降低75%
			FinalDamage *= 0.1f;
		}
	}
	
	
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

		if (MaxEnergy > 0.f)
		{
			// 说明是玩家角色
			UHeroCombatComponent* HeroCombatComponent = Cast<UHeroCombatComponent>(UArcaneBlueprintFunctionLibrary::NativeGetPawnCombatComponentFromActor(ExecutionParams.GetOwningSpec().GetContext().GetEffectCauser()));
			if (HeroCombatComponent)
			{
				int32 FinalDamageInt = FMath::RoundToInt(FinalDamage);
				// 触发伤害浮动文本事件
				HeroCombatComponent->ShowDamageFloatingText(FinalDamageInt, bIsCriticalHit);
			}
		}
		
	}
	
}
