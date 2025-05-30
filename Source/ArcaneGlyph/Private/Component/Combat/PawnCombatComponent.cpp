// INVI_1998 All Rights Reserved.


#include "Component/Combat/PawnCombatComponent.h"

#include "Characters/ArcaneCharacterBase.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/ArcaneWeaponBase.h"


void UPawnCombatComponent::ShowDamageFloatingText(int32 InDamageValue, bool bIsCriticalHit) const
{
	AArcaneCharacterBase* Character = Cast<AArcaneCharacterBase>(GetOwningPawn());
	if (!Character) return;

	
	if (DamageHitLocation.IsZero())
	{
		FVector HitLocation = Character->GetActorLocation() + Character->GetActorForwardVector() * 100.f;
		OnCauseDamage.Broadcast(HitLocation, InDamageValue, bIsCriticalHit);
	}
	else
	{
		// 触发伤害浮动文本事件
		OnCauseDamage.Broadcast(DamageHitLocation, InDamageValue, bIsCriticalHit);
	}
	
}

void UPawnCombatComponent::RegisterSpawnedWeapon(const FGameplayTag& InWeaponTag, AArcaneWeaponBase* InWeapon, bool bEquipped, bool bLeftHand)
{
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTag), TEXT("Weapon %s already exists in the map!"), *InWeaponTag.ToString());
	check(InWeapon);

	CharacterCarriedWeaponMap.Emplace(InWeaponTag, InWeapon);

	// 因为我们的武器击中委托是原生多播委托，所以绑定的函数可以不必是UFUNCTION
	InWeapon->OnWeaponHitTarget.BindUObject(this, &UPawnCombatComponent::OnHitTargetActor);
	InWeapon->OnWeaponPulledTarget.BindUObject(this, &UPawnCombatComponent::OnWeaponPulledFromTargetActor);

	if (bEquipped)
	{
		if (bLeftHand)
		{
			CurrentEquippedLeftHandWeaponTag = InWeaponTag;
		}
		else
		{
			CurrentEquippedRightHandWeaponTag = InWeaponTag;
		}
	}
}

AArcaneWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeapon(const FGameplayTag& InWeaponTag) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTag))
	{
		if (AArcaneWeaponBase* Weapon = CharacterCarriedWeaponMap.FindRef(InWeaponTag))
		{
			return Weapon;
		}
	}
	return nullptr;
}

AArcaneWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon(bool bLeftHand) const
{
	if (bLeftHand)
	{
		return CurrentEquippedLeftHandWeaponTag.IsValid() ? GetCharacterCarriedWeapon(CurrentEquippedLeftHandWeaponTag) : nullptr;
	}
	else
	{
		return CurrentEquippedRightHandWeaponTag.IsValid() ? GetCharacterCarriedWeapon(CurrentEquippedRightHandWeaponTag) : nullptr;
	}
}

bool UPawnCombatComponent::IsCharacterEquippedWeapon() const
{
	return CurrentEquippedRightHandWeaponTag.IsValid() || CurrentEquippedLeftHandWeaponTag.IsValid();
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bEnable, EToggleDamageType InToggleDamageType)
{
	if (InToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		ToggleCurrentEquippedWeaponCollision(bEnable);
	}
	else if (InToggleDamageType == EToggleDamageType::LeftHandWeapon)
	{
		ToggleCurrentEquippedWeaponCollision(bEnable, true);
	}
	else if (InToggleDamageType == EToggleDamageType::RightHandWeapon)
	{
		ToggleCurrentEquippedWeaponCollision(bEnable, false);
	}
	else
	{
		ToggleBodyCollisionBoxCollision(bEnable, InToggleDamageType);
	}
}

void UPawnCombatComponent::OnHitTargetActor(AActor* InHitActor, int32 InCollisionBoxIndex, FVector InHitLocation)
{
	DamageHitLocation = InHitLocation;
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InHitActor, int32 InCollisionBoxIndex)
{
}

void UPawnCombatComponent::ToggleCurrentEquippedWeaponCollision(bool bEnable, bool bLeftHandWeapon)
{
	if (AArcaneWeaponBase* CurrentEquippedWeapon = GetCharacterCurrentEquippedWeapon(bLeftHandWeapon))
	{
		CurrentEquippedWeapon->ToggleWeaponCollision(bEnable);
	}

	if (!bEnable)
	{
		HitOverlappedActors.Empty();
	}
}

void UPawnCombatComponent::ToggleBodyCollisionBoxCollision(bool bEnable, EToggleDamageType InToggleDamageType)
{
	if (!bEnable)
	{
		HitOverlappedActors.Empty();
	}

	AArcaneCharacterBase* EnemyCharacter = Cast<AArcaneCharacterBase>(GetOwningPawn());
	if (!EnemyCharacter) return;

	if (InToggleDamageType == EToggleDamageType::LeftHand)
	{
		EnemyCharacter->GetLeftHandCollisionBox()->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
	else if (InToggleDamageType == EToggleDamageType::RightHand)
	{
		EnemyCharacter->GetRightHandCollisionBox()->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
	else if (InToggleDamageType == EToggleDamageType::Head)
	{
		EnemyCharacter->GetHeadCollisionBox()->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
	else if (InToggleDamageType == EToggleDamageType::LeftFoot)
	{
		EnemyCharacter->GetLeftFootCollision()->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
	else if (InToggleDamageType == EToggleDamageType::RightFoot)
	{
		EnemyCharacter->GetRightFootCollision()->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
}

