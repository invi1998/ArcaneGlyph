// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ArcaneAttributeSet.h"

#include "GameplayEffectExtension.h"

UArcaneAttributeSet::UArcaneAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UArcaneAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// 获取当前生命值和最大生命值
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth()));
	}
	// 获取当前怒气值和最大怒气值
	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		SetCurrentRage(FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage()));
	}

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();
		SetCurrentHealth(FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth()));

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("OldHealth: %f, DamageDone: %f, CurrentHealth: %f"), OldHealth, DamageDone, GetCurrentHealth()));
	}
	
}
