// INVI_1998 All Rights Reserved.


#include "AbilitySystem/AbilityTasks/AbilityTask_WaitSpawnEnemies.h"

#include "AbilitySystemComponent.h"

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

void UAbilityTask_WaitSpawnEnemies::Activate()
{
	FGameplayEventMulticastDelegate& EventDelegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);

	OnEnemiesSpawnedDelegateHandle = EventDelegate.AddUObject(this, &UAbilityTask_WaitSpawnEnemies::OnGameplayEventReceived);
	
}

void UAbilityTask_WaitSpawnEnemies::OnDestroy(bool bInOwnerFinished)
{

	FGameplayEventMulticastDelegate& EventDelegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
	EventDelegate.Remove(OnEnemiesSpawnedDelegateHandle);
	
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitSpawnEnemies::OnGameplayEventReceived(const FGameplayEventData* GameplayEventData)
{
}

