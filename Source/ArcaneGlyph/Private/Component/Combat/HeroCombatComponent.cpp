// INVI_1998 All Rights Reserved.


#include "Component/Combat/HeroCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneDebugHelper.h"
#include "ArcaneGameplayTags.h"
#include "GameplayEffect.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "Characters/ArcaneHeroCharacter.h"
#include "Component/UI/HeroUIComponent.h"
#include "Items/Weapons/ArcaneHeroWeapon.h"

void UHeroCombatComponent::BroadcastCurrentCombatState(const UHeroUIComponent* InHeroUIComponent) const
{
	UArcaneAbilitySystemComponent* ArcaneASC = UArcaneBlueprintFunctionLibrary::GetArcaneASCFromActor(GetOwningPawn());
	if (InHeroUIComponent && ArcaneASC)
	{
		// 获取当前装备的武器
		if (const AArcaneHeroWeapon* CurrentEquippedWeapon = GetHeroCurrentEquippedWeapon())
		{
			const FArcaneHeroWeaponData& WeaponData = CurrentEquippedWeapon->HeroWeaponData;
			// 广播当前武器图标
			InHeroUIComponent->OnEquippedWeaponChanged.Broadcast(
				WeaponData.WeaponIcon
			);

			// 遍历当前武器技能，广播技能状态
			for (const FArcaneHeroSpecialAbilitySet& SpecialAbility : WeaponData.SpecialWeaponAbilities)
			{
				InHeroUIComponent->OnAbilityIconSlotUpdated.Broadcast(SpecialAbility.InputTag, SpecialAbility.SoftAbilityIconTexture2D);

				// 广播当前技能冷却时间
				float TotalCooldownTime = 0.f;
				float RemainingCooldownTime = 0.f;
				if (GetAbilityRemainingCooldownTimeByTag(ArcaneASC, SpecialAbility.AbilityCooldownTag, TotalCooldownTime, RemainingCooldownTime))
				{
					InHeroUIComponent->OnAbilityCooldownBegin.Broadcast(
						SpecialAbility.InputTag,
						TotalCooldownTime,
						RemainingCooldownTime
					);
				}
			}

			// 广播当前武器的攻击类型（棍法）
			TSoftObjectPtr<UTexture2D> TextureStaffIcon = WeaponData.AbilitySetGroups.Find(CurrentComboTypeTag)->GroupIcon;
			InHeroUIComponent->OnComboTypeChanged.Broadcast(
				CurrentComboTypeTag,
				TextureStaffIcon
			);

			InHeroUIComponent->OnCurrentUsedItemTagChanged.Broadcast(CurrentUsedItemTag);
		}
		else
		{
			InHeroUIComponent->OnEquippedWeaponChanged.Broadcast(
				nullptr
			);
		}
	}
}

void UHeroCombatComponent::StartEnergyRegenTimer()
{
	// 获取世界上下文
	UWorld* World = GetWorld();
	if (!World) return;

	// 检查并管理计时器
	FTimerManager& TimerManager = World->GetTimerManager();
    
	if (TimerManager.IsTimerActive(EnergyRegenTimerHandle))
	{
		// 重置现有计时器
		TimerManager.SetTimer(
			EnergyRegenTimerHandle,
			this,
			&UHeroCombatComponent::EnergyRegen,
			EnergyRegenDelay,
			false
		);
	}
	else
	{
		// 创建新计时器
		TimerManager.SetTimer(
			EnergyRegenTimerHandle,
			this,
			&UHeroCombatComponent::EnergyRegen,
			EnergyRegenDelay,
			false
		);
	}
}

void UHeroCombatComponent::ClearEnergyRegenTimer()
{
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(EnergyRegenTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(EnergyRegenTimerHandle);
	}
}

void UHeroCombatComponent::SetEnergyRegenCooldownTag()
{
	UArcaneBlueprintFunctionLibrary::AddGameplayTagToActorIfNotHas(GetOwningPawn(), ArcaneGameplayTags::Player_Status_EnergyRegenCooldown);
}

void UHeroCombatComponent::EnergyRegen()
{
	UArcaneBlueprintFunctionLibrary::RemoveGameplayTagFromActorIfHas(GetOwningPawn(), ArcaneGameplayTags::Player_Status_EnergyRegenCooldown);
}

void UHeroCombatComponent::ChangeCurrentComboTypeTag(AArcaneHeroCharacter* InHeroCharacter, const FGameplayTag& InComboTypeTag)
{
	CurrentComboTypeTag = InComboTypeTag;
	if (UHeroUIComponent* HeroUI = InHeroCharacter->GetHeroUIComponent())
	{
		if (const AArcaneHeroWeapon* CurrentEquippedWeapon = GetHeroCurrentEquippedWeapon())
		{
			const FArcaneHeroWeaponData& WeaponData = CurrentEquippedWeapon->HeroWeaponData;
			TSoftObjectPtr<UTexture2D> IconTexture2D = WeaponData.AbilitySetGroups.Find(InComboTypeTag)->GroupIcon;
			
			HeroUI->OnComboTypeChanged.Broadcast(InComboTypeTag, IconTexture2D);
		}
		
	}
}

void UHeroCombatComponent::SetCurrentUsedItemTag(AArcaneHeroCharacter* InHeroCharacter, const FGameplayTag& InItemTag)
{
	CurrentUsedItemTag = InItemTag;
	if (UHeroUIComponent* HeroUI = InHeroCharacter->GetHeroUIComponent())
	{
		HeroUI->OnCurrentUsedItemTagChanged.Broadcast(InItemTag);
	}
}

void UHeroCombatComponent::SwitchCurrentUseItem_Left(AArcaneHeroCharacter* InHeroCharacter)
{
	if (CurrentUsedItemTag == ArcaneGameplayTags::InputTag_UseItem_HealPotion)
	{
		SetCurrentUsedItemTag(InHeroCharacter, ArcaneGameplayTags::InputTag_UseItem_ExtraDamagePotion);
	}
	else if (CurrentUsedItemTag == ArcaneGameplayTags::InputTag_UseItem_ExtraDamagePotion)
	{
		SetCurrentUsedItemTag(InHeroCharacter, ArcaneGameplayTags::InputTag_UseItem_ManaPotion);
	}
	else if (CurrentUsedItemTag == ArcaneGameplayTags::InputTag_UseItem_ManaPotion)
	{
		SetCurrentUsedItemTag(InHeroCharacter, ArcaneGameplayTags::InputTag_UseItem_RagePotion);
	}
	else if (CurrentUsedItemTag == ArcaneGameplayTags::InputTag_UseItem_RagePotion)
	{
		SetCurrentUsedItemTag(InHeroCharacter, ArcaneGameplayTags::InputTag_UseItem_HealPotion);
	}
	else
	{
		SetCurrentUsedItemTag(InHeroCharacter, ArcaneGameplayTags::InputTag_UseItem_HealPotion);
	}
}

void UHeroCombatComponent::SwitchCurrentUseItem_Right(AArcaneHeroCharacter* InHeroCharacter)
{
	if (CurrentUsedItemTag == ArcaneGameplayTags::InputTag_UseItem_HealPotion)
	{
		SetCurrentUsedItemTag(InHeroCharacter, ArcaneGameplayTags::InputTag_UseItem_RagePotion);
	}
	else if (CurrentUsedItemTag == ArcaneGameplayTags::InputTag_UseItem_RagePotion)
	{
		SetCurrentUsedItemTag(InHeroCharacter, ArcaneGameplayTags::InputTag_UseItem_ManaPotion);
	}
	else if (CurrentUsedItemTag == ArcaneGameplayTags::InputTag_UseItem_ManaPotion)
	{
		SetCurrentUsedItemTag(InHeroCharacter, ArcaneGameplayTags::InputTag_UseItem_ExtraDamagePotion);
	}
	else if (CurrentUsedItemTag == ArcaneGameplayTags::InputTag_UseItem_ExtraDamagePotion)
	{
		SetCurrentUsedItemTag(InHeroCharacter, ArcaneGameplayTags::InputTag_UseItem_HealPotion);
	}
	else
	{
		SetCurrentUsedItemTag(InHeroCharacter, ArcaneGameplayTags::InputTag_UseItem_HealPotion);
	}
}

AArcaneHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(const FGameplayTag& InWeaponTag) const
{
	return Cast<AArcaneHeroWeapon>(GetCharacterCarriedWeapon(InWeaponTag));
}

AArcaneHeroWeapon* UHeroCombatComponent::GetHeroCurrentEquippedWeapon() const
{
	return Cast<AArcaneHeroWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UHeroCombatComponent::GetHeroCurrenEquippedWeaponDamageAtLevel(float InLevel) const
{
	if (const AArcaneHeroWeapon* CurrentEquippedWeapon = GetHeroCurrentEquippedWeapon())
	{
		const FArcaneHeroWeaponData& WeaponData = CurrentEquippedWeapon->HeroWeaponData;
		return WeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
	}
	return 0.f;
}

float UHeroCombatComponent::GetHeroCurrenRageGainBaseAtlevel(float InLevel) const
{
	return RageGainBase.GetValueAtLevel(InLevel);
}

void UHeroCombatComponent::OnHitTargetActor(AActor* InHitActor, int32 InCollisionBoxIndex, FVector InHitLocation)
{
	
	if (!IsValid(InHitActor)) return;
	
	if (HitOverlappedActors.Contains(InHitActor)) return;
	HitOverlappedActors.AddUnique(InHitActor);

	Super::OnHitTargetActor(InHitActor, InCollisionBoxIndex, InHitLocation);
	
	// HitOverlappedActors.Add(InHitActor);

	FGameplayEventData EventData;
	EventData.Target = InHitActor;
	EventData.Instigator = GetOwningPawn();

	bool bIsBlocking = UArcaneBlueprintFunctionLibrary::NativeDoesActorHasGameplayTag(InHitActor, ArcaneGameplayTags::Enemy_Status_Blocking);
	bool bIsSuccessBlock = false;
	if (bIsBlocking)
	{
		// 格挡
		bIsSuccessBlock = UArcaneBlueprintFunctionLibrary::IsCurrentBlockValid(GetOwningPawn(), InHitActor);
	}
	
	if (bIsSuccessBlock)
	{
		// 格挡成功
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			InHitActor,
			ArcaneGameplayTags::Enemy_Event_SuccessBlock,
			EventData
		);
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			InHitActor,
			ArcaneGameplayTags::Enemy_Event_FailedBlock,
			EventData
		);
		
		if (InCollisionBoxIndex == 1)
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				GetOwningPawn(),
				ArcaneGameplayTags::Shared_Event_MeleeAttack_1,
				EventData
			);
		}
		else
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				GetOwningPawn(),
				ArcaneGameplayTags::Shared_Event_MeleeAttack_2,
				EventData
			);
		}

		// 发送受击暂停事件
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			ArcaneGameplayTags::Player_Event_HitPause,
			FGameplayEventData()
		);
	}
	
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InHitActor, int32 InCollisionBoxIndex)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		ArcaneGameplayTags::Player_Event_HitPause,
		FGameplayEventData()
	);
}

bool UHeroCombatComponent::GetAbilityRemainingCooldownTimeByTag(const UArcaneAbilitySystemComponent* ASC, const FGameplayTag& InCooldownTag, float& TotalCooldownTime, float& OutRemainingCooldown) const
{
	FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());

	TArray<TPair<float, float>> CooldownList = ASC->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

	if (CooldownList.Num() > 0)
	{
		TotalCooldownTime = CooldownList[0].Value;
		OutRemainingCooldown = CooldownList[0].Key;
		
	}
	else
	{
		TotalCooldownTime = 0;
		OutRemainingCooldown = 0;
	}
	
	return OutRemainingCooldown > 0.f;
}
