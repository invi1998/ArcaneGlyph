// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

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
	
}
