// INVI_1998 All Rights Reserved.


#include "AbilitySystem/AbilityTasks/AbilityTask_WaitSpawnEnemies.h"

#include "AbilitySystemComponent.h"
#include "NavigationSystem.h"
#include "Characters/ArcaneEnemyCharacter.h"
#include "Engine/AssetManager.h"

UAbilityTask_WaitSpawnEnemies* UAbilityTask_WaitSpawnEnemies::WaitSpawnEnemies(UGameplayAbility* OwningAbility,
                                                                               FGameplayTag EventTag, TSoftClassPtr<AArcaneEnemyCharacter> SoftEnemyClassToSpawn, int32 SpawnCount,
                                                                               const FVector& SpawnOrigin, float SpawnRadius)
{
	UAbilityTask_WaitSpawnEnemies* TaskNode = NewAbilityTask<UAbilityTask_WaitSpawnEnemies>(OwningAbility);

	TaskNode->CachedEventTag = EventTag;
	TaskNode->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	TaskNode->CachedSpawnOrigin = SpawnOrigin;
	TaskNode->CachedSpawnRadius = SpawnRadius;
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

void UAbilityTask_WaitSpawnEnemies::OnEnemyClassLoaded()
{
	UClass* EnemyClass = CachedSoftEnemyClassToSpawn.Get();
	UWorld* World = GetWorld();
	if (World && EnemyClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;	// 如果有碰撞，就调整位置，但是总是生成
		
		TArray<AArcaneEnemyCharacter*> SpawnedEnemies;
		for (int32 i = 0; i < CachedSpawnCount; i++)
		{
			// 为了确保敌人生成正常，我们需要确保生成的位置是在NavMesh上的，所以我们需要使用 NavMeshBoundsVolume 来获取一个合适的位置
			FVector SpawnLocation{};
			// 该函数会返回一个在指定半径内的可到达的随机位置
			UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, CachedSpawnOrigin, SpawnLocation, CachedSpawnRadius);
			// 然后我们随机将生成的敌人提升一定的高度，以确保敌人不会被地面卡住，同时达到一个召唤物从天而降的效果
			SpawnLocation.Z += FMath::RandRange(100.f, 200.f);

			// 生成敌人的朝向，我们可以直接使用 AvatarActor 的朝向
			FRotator SpawnFacingRotator = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();
			
			if (AArcaneEnemyCharacter* SpawnedEnemy = World->SpawnActor<AArcaneEnemyCharacter>(EnemyClass, SpawnLocation, SpawnFacingRotator, SpawnParams))
			{
				SpawnedEnemies.Add(SpawnedEnemy);
			}
		}

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			if (SpawnedEnemies.IsEmpty())
			{
				OnEnemiesSpawnedFailed.Broadcast(SpawnedEnemies);
			}
			else
			{
				OnEnemiesSpawnedSuccess.Broadcast(SpawnedEnemies);
			}
		}

		EndTask();
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnEnemiesSpawnedFailed.Broadcast(TArray<AArcaneEnemyCharacter*>());
		}

		EndTask();
	}
}

void UAbilityTask_WaitSpawnEnemies::OnGameplayEventReceived(const FGameplayEventData* GameplayEventData)
{
	// 确保敌人类不为空
	if (ensure(!CachedSoftEnemyClassToSpawn.IsNull()))
	{
		// 异步加载敌人类
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
        		CachedSoftEnemyClassToSpawn.ToSoftObjectPath(),
        		FStreamableDelegate::CreateUObject(this, &UAbilityTask_WaitSpawnEnemies::OnEnemyClassLoaded));
	}
	else
	{
		// 如果敌人类为空，就直接结束任务，并广播失败事件
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnEnemiesSpawnedFailed.Broadcast(TArray<AArcaneEnemyCharacter*>());
		}

		EndTask();
	}

}

