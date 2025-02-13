// INVI_1998 All Rights Reserved.


#include "AbilitySystem/Abilities/ArcaneHeroGameplayAbility.h"

#include "Characters/ArcaneHeroCharacter.h"
#include "Player/ArcaneHeroController.h"

AArcaneHeroCharacter* UArcaneHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if (!CachedArcaneHeroCharacter.IsValid())
	{
		CachedArcaneHeroCharacter = Cast<AArcaneHeroCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedArcaneHeroCharacter.IsValid() ? CachedArcaneHeroCharacter.Get() : nullptr;
}

AArcaneHeroController* UArcaneHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	if (!CachedArcaneHeroController.IsValid())
	{
		CachedArcaneHeroController = Cast<AArcaneHeroController>(CurrentActorInfo->PlayerController);
	}
	return CachedArcaneHeroController.IsValid() ? CachedArcaneHeroController.Get() : nullptr;
}

UHeroCombatComponent* UArcaneHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	if (AArcaneHeroCharacter* HeroCharacter = GetHeroCharacterFromActorInfo())
	{
		return HeroCharacter->GetHeroCombatComponent();
	}
	return nullptr;
}
