// INVI_1998 All Rights Reserved.


#include "AbilitySystem/Abilities/ArcaneEnemyGameplayAbility.h"

#include "Characters/ArcaneEnemyCharacter.h"

AArcaneEnemyCharacter* UArcaneEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedArcaneEnemyCharacter.IsValid())
	{
		CachedArcaneEnemyCharacter = Cast<AArcaneEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedArcaneEnemyCharacter.IsValid() ? CachedArcaneEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UArcaneEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	if (AArcaneEnemyCharacter* EnemyCharacter = GetEnemyCharacterFromActorInfo())
	{
		return EnemyCharacter->GetEnemyCombatComponent();
	}
	return nullptr;
}
