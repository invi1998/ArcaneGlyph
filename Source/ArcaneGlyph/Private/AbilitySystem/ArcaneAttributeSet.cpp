// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ArcaneAttributeSet.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "GameplayEffectExtension.h"
#include "ArcaneGameplayTags.h"
#include "Component/UI/HeroUIComponent.h"
#include "Component/UI/PawnUIComponent.h"
#include "Interfaces/PawnUIInterface.h"

UArcaneAttributeSet::UArcaneAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
	InitDamageTaken(0.f);
	InitHealthPotion(0.f);
	InitMaxHealthPotion(0.f);
	InitRagePotion(0.f);
	InitMaxRagePotion(0.f);
	InitMaxSpark(0.f);
	InitCurrentSpark(0.f);
	InitRageBaseIncrement(100.f);
}

void UArcaneAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// 如果当前角色没有缓存的 UI 接口，则尝试获取
	if (!CachedPawnUIInterface.IsValid())
	{
		// 如果当前角色实现了 IPawnUIInterface 接口，则缓存
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}
	checkf(CachedPawnUIInterface.IsValid(), TEXT("%s: PawnUIInterface is not implemented in %s!"), *FString(__FUNCTION__), *Data.Target.GetAvatarActor()->GetName());
	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	UEnemyUIComponent* EnemyUIComponent = CachedPawnUIInterface->GetEnemyUIComponent();
	checkf(PawnUIComponent, TEXT("%s: PawnUIComponent is null in %s!"), *FString(__FUNCTION__), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());
	
	// 获取当前生命值和最大生命值
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth()));
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	}
	// 获取当前怒气值和最大怒气值
	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		SetCurrentRage(FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage()));

		int32 Spark = CalculateCurrentSpark();
		if (Spark != GetCurrentSpark())
		{
			SetCurrentSpark(FMath::Clamp(Spark, 0.f, GetMaxSpark()));
			if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
			{
				HeroUIComponent->OnCurrentSparkNumChanged.Broadcast(GetCurrentSpark(), GetMaxSpark());
			}
		}
		
		if (GetCurrentSpark() >= 1)
		{
			UArcaneBlueprintFunctionLibrary::AddGameplayTagToActorIfNotHas(Data.Target.GetAvatarActor(), ArcaneGameplayTags::Player_Status_Rage_Full);
		}
		else if (GetCurrentSpark() <= 0.f)
		{
			UArcaneBlueprintFunctionLibrary::AddGameplayTagToActorIfNotHas(Data.Target.GetAvatarActor(), ArcaneGameplayTags::Player_Status_Rage_None);
		}
		else
		{
			UArcaneBlueprintFunctionLibrary::RemoveGameplayTagFromActorIfHas(Data.Target.GetAvatarActor(), ArcaneGameplayTags::Player_Status_Rage_Full);
			UArcaneBlueprintFunctionLibrary::RemoveGameplayTagFromActorIfHas(Data.Target.GetAvatarActor(), ArcaneGameplayTags::Player_Status_Rage_None);
		}

		if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
		{
			HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentSegmentRagePercent());
		}
	}

	// 获取当前生命药水和最大生命药水
	if (Data.EvaluatedData.Attribute == GetHealthPotionAttribute())
	{
		if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
		{
			SetHealthPotion(FMath::Clamp(GetHealthPotion(), 0.f, GetMaxHealthPotion()));
			HeroUIComponent->OnCurrentHealthPotionChanged.Broadcast(GetHealthPotion(), GetMaxHealthPotion());
		}
		
	}
	
	// 获取当前怒气药水和最大怒气药水
	if (Data.EvaluatedData.Attribute == GetRagePotionAttribute())
	{
		if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
		{
			SetRagePotion(FMath::Clamp(GetRagePotion(), 0.f, GetMaxRagePotion()));
			HeroUIComponent->OnCurrentRagePotionChanged.Broadcast(GetRagePotion(), GetMaxRagePotion());
		}
		
	}

	// 获取当前火花点和最大火花点
	if (Data.EvaluatedData.Attribute == GetCurrentSparkAttribute())
	{
		if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
		{
			SetCurrentSpark(FMath::Clamp(GetCurrentSpark(), 0.f, GetMaxSpark()));
			HeroUIComponent->OnCurrentSparkNumChanged.Broadcast(GetCurrentSpark(), GetMaxSpark());
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();
		SetCurrentHealth(FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth()));
		
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
		
		if (GetCurrentHealth() <= 0.f)
		{
			// 如果当前生命值小于等于0，则添加死亡标签
			UArcaneBlueprintFunctionLibrary::AddGameplayTagToActorIfNotHas(Data.Target.GetAvatarActor(), ArcaneGameplayTags::Shared_Status_Dead);
		}
	}
	
}

int32 UArcaneAttributeSet::CalculateCurrentSpark() const
{
	float CurrentValue = GetCurrentRage();
	int32 CalculatedSpark = 0;
	for (int32 Tier = 1; Tier <= GetMaxSpark(); ++Tier)
	{
		float TierThreshold = GetRageBaseIncrement() * Tier * (Tier + 1) / 2;
		if (CurrentValue >= TierThreshold)
		{
			CalculatedSpark = Tier;
		}
		else
		{
			break;
		}
	}
	return CalculatedSpark;
}

float UArcaneAttributeSet::GetCurrentSegmentRagePercent() const
{
	// 边界条件处理
	if (GetCurrentSpark() <= 0 || GetCurrentSpark() > GetMaxSpark() || GetRageBaseIncrement() <= 0)
	{
		return 0.f;
	}

	// 计算当前段的起始和结束阈值
	const int32 Tier = GetCurrentSpark();
	const float StartThreshold = (Tier - 1) * Tier * GetRageBaseIncrement() / 2.f;
	const float EndThreshold = Tier * (Tier + 1) * GetRageBaseIncrement() / 2.f;

	// 避免除以零 FMath::IsNearlyZero(EndThreshold - StartThreshold)
	if (FMath::IsNearlyEqual(EndThreshold, StartThreshold))
	{
		return (GetCurrentRage() >= EndThreshold) ? 1.f : 0.f;
	}

	// 计算当前段的百分比（自动钳制范围）
	const float ClampedMomentum = FMath::Clamp(GetCurrentRage(), StartThreshold, EndThreshold);
	const float RagePercent = (ClampedMomentum - StartThreshold) / (EndThreshold - StartThreshold);
	// 保留两位小数
	// return FMath::RoundToFloat(RagePercent * 100) / 100;
	return RagePercent;
	
}
