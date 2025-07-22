// INVI_1998 All Rights Reserved.


#include "AbilitySystem/ArcaneAttributeSet.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneDebugHelper.h"
#include "GameplayEffectExtension.h"
#include "ArcaneGameplayTags.h"
#include "Component/Combat/HeroCombatComponent.h"
#include "Component/Combat/PawnCombatComponent.h"
#include "Component/UI/HeroUIComponent.h"
#include "Component/UI/PawnUIComponent.h"
#include "Interfaces/ArcaneStatusInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"
#include "Items/Weapons/ArcaneHeroWeapon.h"
#include "Kismet/KismetMathLibrary.h"

UArcaneAttributeSet::UArcaneAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(480.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
	InitDamageTaken(0.f);
	InitHealthPotion(0.f);
	InitMaxHealthPotion(0.f);
	InitRagePotion(0.f);
	InitMaxRagePotion(0.f);
	InitMaxSpark(4.f);
	InitCurrentSpark(0.f);
	InitRageBaseIncrement(100.f);
	InitExtraRageIncrement(10.f);
	InitCurrentEnergy(0.f);
	InitMaxEnergy(0.f);
	InitCurrentMana(0.f);
	InitMaxMana(0.f);
	InitManaPotion(0.f);
	InitMaxManaPotion(0.f);
	InitExtraDamage(0.f);
	InitDamagePotion(0.f);
	InitMaxDamagePotion(0.f);
}

void UArcaneAttributeSet::BroadcastCharacterAttributeInfo(const UHeroUIComponent* InHeroUIComponent) const
{
	if (InHeroUIComponent)
	{
		InHeroUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
		InHeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentSegmentRagePercent(), GetCurrentSpark(), GetMaxSpark());
		InHeroUIComponent->OnCurrentEnergyChanged.Broadcast(UKismetMathLibrary::SafeDivide(GetCurrentEnergy(), GetMaxEnergy()));
		InHeroUIComponent->OnCurrentSparkNumChanged.Broadcast(GetCurrentSpark(), GetMaxSpark());
		InHeroUIComponent->OnCurrentHealthPotionChanged.Broadcast(GetHealthPotion(), GetMaxHealthPotion());
		InHeroUIComponent->OnCurrentRagePotionChanged.Broadcast(GetRagePotion(), GetMaxRagePotion());
		InHeroUIComponent->OnCurrentManaChanged.Broadcast(UKismetMathLibrary::SafeDivide(GetCurrentMana(), GetMaxMana()));
		InHeroUIComponent->OnCurrentManaPotionChanged.Broadcast(GetManaPotion(), GetMaxManaPotion());
		InHeroUIComponent->OnCurrentExtraDamagePotionChanged.Broadcast(GetDamagePotion(), GetMaxDamagePotion());
	}
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

	if (!CachedPawnCombatInterface.IsValid())
	{
		CachedPawnCombatInterface = TWeakInterfacePtr<IPawnCombatInterface>(Data.Target.GetAvatarActor());
	}
	checkf(CachedPawnCombatInterface.IsValid(), TEXT("%s: PawnCombatInterface is not implemented in %s!"), *FString(__FUNCTION__), *Data.Target.GetAvatarActor()->GetName());
	UPawnCombatComponent* PawnCombatComponent = CachedPawnCombatInterface->GetPawnCombatComponent();
	checkf(PawnCombatComponent, TEXT("%s: PawnCombatComponent is null in %s!"), *FString(__FUNCTION__), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());

	// 气力变化
	if (Data.EvaluatedData.Attribute == GetCurrentEnergyAttribute())
	{
		SetCurrentEnergy(FMath::Clamp(GetCurrentEnergy(), 0.f, GetMaxEnergy()));
		if (GetCurrentEnergy() <= 0.f)
		{
			// 获取当前角色的ASC
			if (UArcaneAbilitySystemComponent* ArcaneASC = UArcaneBlueprintFunctionLibrary::NativeGetArcaneASCFromActor(Data.Target.GetAvatarActor()))
			{
				ArcaneASC->SendChargeComplete();
			}
		}
		
		if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
		{
			float TempCurrentEnergyPercent = UKismetMathLibrary::SafeDivide(GetCurrentEnergy(), GetMaxEnergy());
			HeroUIComponent->OnCurrentEnergyChanged.Broadcast(TempCurrentEnergyPercent);
		}
	}

	// 法力值变化
	if (Data.EvaluatedData.Attribute == GetCurrentManaAttribute())
	{
		SetCurrentMana(FMath::Clamp(GetCurrentMana(), 0.f, GetMaxMana()));
		if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
		{
			float TempCurrentManaPercent = UKismetMathLibrary::SafeDivide(GetCurrentMana(), GetMaxMana());
			HeroUIComponent->OnCurrentManaChanged.Broadcast(TempCurrentManaPercent);
		}
	}
	
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
		if (Spark <= GetMaxSpark() && Spark != GetCurrentSpark())
		{
			SetCurrentSpark(FMath::Clamp(Spark, 0, GetMaxSpark()));
			if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
			{
				HeroUIComponent->OnCurrentSparkNumChanged.Broadcast(GetCurrentSpark(), GetMaxSpark());
			}
			// 给玩家武器发送棍势点改变事件
			if (AArcaneHeroWeapon* PlayerWeapon = Cast<AArcaneHeroWeapon>(PawnCombatComponent->GetCharacterCurrentEquippedWeapon()))
			{
				if (PlayerWeapon->GetClass()->ImplementsInterface(UArcaneStatusInterface::StaticClass()))
				{
					IArcaneStatusInterface::Execute_OnSparkChanged(PlayerWeapon, GetCurrentSpark());
				}
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
			// 打印角色当前怒气值和最大值
			HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentSegmentRagePercent(), GetCurrentSpark(), GetMaxSpark());
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

	// 获取当前法力药水和最大法力药水
	if (Data.EvaluatedData.Attribute == GetManaPotionAttribute())
	{
		if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
		{
			SetManaPotion(FMath::Clamp(GetManaPotion(), 0.f, GetMaxManaPotion()));
			HeroUIComponent->OnCurrentManaPotionChanged.Broadcast(GetManaPotion(), GetMaxManaPotion());
		}
		
	}

	// 获取当前额外伤害药水和最大额外伤害药水
	if (Data.EvaluatedData.Attribute == GetDamagePotionAttribute())
	{
		if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
		{
			SetDamagePotion(FMath::Clamp(GetDamagePotion(), 0.f, GetMaxDamagePotion()));
			HeroUIComponent->OnCurrentExtraDamagePotionChanged.Broadcast(GetDamagePotion(), GetMaxDamagePotion());
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

float UArcaneAttributeSet::CalculateTierThreshold(int32 Tier) const
{
	if (Tier <= 0) return 0.f;

	// 公式：Threshold(n) = BaseIncrement * n + ExtraIncrement * (n(n-1)/2)
	return GetRageBaseIncrement() * Tier + GetExtraRageIncrement() * (Tier * (Tier - 1) / 2.0f);
}

int32 UArcaneAttributeSet::CalculateCurrentSpark() const
{
	float CurrentValue = GetCurrentRage();
	int32 CalculatedSpark = 0;
	for (int32 Tier = 1; Tier <= GetMaxSpark(); ++Tier)
	{
		float TierThreshold = CalculateTierThreshold(Tier);
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
	const int32 CurrentTier = CalculateCurrentSpark();
	const float CurrentValue = GetCurrentRage();

	// 计算当前段起始和结束阈值
	const float StartThreshold = (CurrentTier == 0) ? 0.0f : CalculateTierThreshold(CurrentTier);
	const float EndThreshold = CalculateTierThreshold(CurrentTier + 1);

	// 边界处理
	if (CurrentTier >= GetMaxSpark()) return 1.0f; // 已达最大值
	if (FMath::IsNearlyZero(EndThreshold - StartThreshold)) return 0.0f;

	// 计算百分比
	const float ClampedValue = FMath::Clamp(CurrentValue, StartThreshold, EndThreshold);
	const float Percentage = (ClampedValue - StartThreshold) / (EndThreshold - StartThreshold);
	return Percentage;
	
}
