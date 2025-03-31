// INVI_1998 All Rights Reserved.


#include "Items/PickUp/ArcaneStoneBase.h"

#include "ArcaneGameplayTags.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "Characters/ArcaneHeroCharacter.h"

void AArcaneStoneBase::Consume(UArcaneAbilitySystemComponent* AbilitySystemComponent, int32 InApplyLevel)
{
	check(StoneEffectClass);

	UGameplayEffect* StoneEffect = StoneEffectClass->GetDefaultObject<UGameplayEffect>();
	
	AbilitySystemComponent->ApplyGameplayEffectToSelf(
		StoneEffect,
		InApplyLevel,
		AbilitySystemComponent->MakeEffectContext()
	);

	BP_OnSoulConsume();
}

void AArcaneStoneBase::ConsumeSoul(UArcaneAbilitySystemComponent* AbilitySystemComponent, int32 InApplyLevel)
{
	Consume(AbilitySystemComponent, InApplyLevel);
}

