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

		if (GetCurrentRage() >= GetMaxRage())
		{
			UArcaneBlueprintFunctionLibrary::AddGameplayTagToActorIfNotHas(Data.Target.GetAvatarActor(), ArcaneGameplayTags::Player_Status_Rage_Full);
		}
		else if (GetCurrentRage() <= 0.f)
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
			HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
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
