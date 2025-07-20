// INVI_1998 All Rights Reserved.


#include "Component/UI/HeroUIComponent.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "AbilitySystem/ArcaneAttributeSet.h"
#include "Characters/ArcaneHeroCharacter.h"
#include "Component/Combat/HeroCombatComponent.h"


void UHeroUIComponent::BroadcastInitialValues() const
{
	if (AArcaneHeroCharacter* HeroCharacter = Cast<AArcaneHeroCharacter>(GetOwner()))
	{
		if (UArcaneAbilitySystemComponent* ArcaneASC = UArcaneBlueprintFunctionLibrary::GetArcaneASCFromActor(HeroCharacter))
		{
			if (UArcaneAttributeSet* ArcaneAttributeSet = HeroCharacter->GetArcaneAttributeSet())
			{
				// 广播属性
				ArcaneAttributeSet->BroadcastCharacterAttributeInfo(this);
			}

			if (UHeroCombatComponent* HeroCombatComponent = HeroCharacter->GetHeroCombatComponent())
			{
				// 广播战斗组件的属性
				HeroCombatComponent->BroadcastCurrentCombatState(this);
			}
		}
	}
}
