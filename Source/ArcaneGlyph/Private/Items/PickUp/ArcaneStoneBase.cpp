// INVI_1998 All Rights Reserved.


#include "Items/PickUp/ArcaneStoneBase.h"

#include "ArcaneGameplayTags.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "Characters/ArcaneHeroCharacter.h"


void AArcaneStoneBase::OnPickUpCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AArcaneHeroCharacter* HeroCharacter = Cast<AArcaneHeroCharacter>(OtherActor))
	{
		// 如果是英雄角色，就拾取
	 	UArcaneAbilitySystemComponent* ArcaneASC = HeroCharacter->GetArcaneAbilitySystemComponent();
		if (ArcaneASC)
		{
			ArcaneASC->TryActivateAbilityByTag(ArcaneGameplayTags::Player_Ability_Pickup_Soul);
		}
	}
	
}
