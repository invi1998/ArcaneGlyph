// INVI_1998 All Rights Reserved.


#include "AbilitySystem/AbilityTasks/AbilityTask_WaitSpawnEnemies.h"

UAbilityTask_WaitSpawnEnemies* UAbilityTask_WaitSpawnEnemies::WaitSpawnEnemies(UGameplayAbility* OwningAbility,
	FGameplayTag EventTag, TSoftClassPtr<AArcaneEnemyCharacter> SoftEnemyClassToSpawn, int32 SpawnCount,
	const FVector& SpawnOrigin, float SpawnRadius, const FRotator& SpawnRotation)
{
	UAbilityTask_WaitSpawnEnemies* TaskNode = NewAbilityTask<UAbilityTask_WaitSpawnEnemies>(OwningAbility);

	TaskNode->CachedEventTag = EventTag;
	TaskNode->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	TaskNode->CachedSpawnOrigin = SpawnOrigin;
	TaskNode->CachedSpawnRadius = SpawnRadius;
	TaskNode->CachedSpawnRotation = SpawnRotation;
	TaskNode->CachedSpawnCount = SpawnCount;

	return TaskNode;
	
}
