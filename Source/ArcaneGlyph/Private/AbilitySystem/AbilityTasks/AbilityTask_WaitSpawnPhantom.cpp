// INVI_1998 All Rights Reserved.


#include "AbilitySystem/AbilityTasks/AbilityTask_WaitSpawnPhantom.h"

#include "AbilitySystemComponent.h"
#include "Characters/ArcaneHeroCharacter.h"
#include "Characters/PlayerPhantom.h"
#include "Controllers/ArcaneHeroController.h"
#include "Engine/AssetManager.h"

UAbilityTask_WaitSpawnPhantom* UAbilityTask_WaitSpawnPhantom::WaitSpawnPhantom(
		UGameplayAbility* OwningAbility, FGameplayTag EventTag,
		AArcaneHeroCharacter* InOriginalCharacter,AArcaneHeroController* InHeroController,
		FGenericTeamId TargetTeamID, TSoftClassPtr<APlayerPhantom> SoftPhantomClassToSpawn, float InSpawnDuration)
{
	UAbilityTask_WaitSpawnPhantom* TaskNode = NewAbilityTask<UAbilityTask_WaitSpawnPhantom>(OwningAbility);
	
	TaskNode->CachedEventTag = EventTag;
	TaskNode->SoftPhantomClassToSpawn = SoftPhantomClassToSpawn;
	TaskNode->OriginalCharacter = InOriginalCharacter;
	TaskNode->HeroController = InHeroController;
	TaskNode->TargetTeamID = TargetTeamID;
	TaskNode->SpawnDuration = InSpawnDuration;

	return TaskNode;
}

void UAbilityTask_WaitSpawnPhantom::Activate()
{
	FGameplayEventMulticastDelegate& EventDelegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
	OnPhantomSpawnedDelegateHandle = EventDelegate.AddUObject(this, &UAbilityTask_WaitSpawnPhantom::OnGameplayEventReceived);
}

void UAbilityTask_WaitSpawnPhantom::OnDestroy(bool bInOwnerFinished)
{
	FGameplayEventMulticastDelegate& EventDelegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
	EventDelegate.Remove(OnPhantomSpawnedDelegateHandle);
	
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitSpawnPhantom::OnPhantomClassLoaded()
{
	UClass* PhantomClass = SoftPhantomClassToSpawn.Get();
	UWorld* World = GetWorld();
	if (World && PhantomClass && OriginalCharacter)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; // 如果有碰撞，就调整位置，但是总是生成
		
		FVector SpawnLocation = OriginalCharacter->GetActorLocation();
		FRotator SpawnRotation = OriginalCharacter->GetActorRotation();

		if (APlayerPhantom* SpawnedPhantom = World->SpawnActor<APlayerPhantom>(PhantomClass, SpawnLocation, SpawnRotation, SpawnParams))
		{
			SpawnedPhantom->InitializePhantom(OriginalCharacter, SpawnDuration);
			SpawnedPhantom->SetGenericTeamId(HeroController->GetGenericTeamId()); // 设置幻影的团队 ID
			HeroController->SetGenericTeamId(TargetTeamID); // 设置玩家控制器的团队 ID
			OnPhantomSpawnedSuccess.Broadcast(SpawnedPhantom);
		}
		else
		{
			OnPhantomSpawnedFailed.Broadcast(nullptr);
		}
		
		EndTask();
	}
	else
	{
		OnPhantomSpawnedFailed.Broadcast(nullptr);
		EndTask();
	}
}

void UAbilityTask_WaitSpawnPhantom::OnGameplayEventReceived(const FGameplayEventData* Payload)
{
	// 确保 SoftPhantomClassToSpawn 有效
	if (ensure(!SoftPhantomClassToSpawn.IsNull()))
	{
		// 异步加载幻影类
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		SoftPhantomClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnPhantomClassLoaded)
		);
	}
	else
	{
		// 如果 SoftPhantomClassToSpawn 为空，则直接调用失败委托
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnPhantomSpawnedFailed.Broadcast(nullptr);
		}

		EndTask();
	}
}
