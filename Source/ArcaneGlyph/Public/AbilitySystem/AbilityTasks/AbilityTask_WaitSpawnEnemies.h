// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitSpawnEnemies.generated.h"

class AArcaneEnemyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitSpawnEnemeisDelegate, const TArray<AArcaneEnemyCharacter*>&, SpawnedEnemies);

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UAbilityTask_WaitSpawnEnemies : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Arcane | AbilityTasks",
		meta = (DisplayName = "Wait Gameplay Event And Spawn Enemies",
				HidePin="OwningAbility",
				DefaultToSelf="OwningAbility",
				BlueprintInternalUseOnly = "TRUE",
				SpawnCount = "1",
				SpawnRadius = "300.f"))
	static UAbilityTask_WaitSpawnEnemies* WaitSpawnEnemies(
		UGameplayAbility* OwningAbility,
		FGameplayTag EventTag,
		TSoftClassPtr<AArcaneEnemyCharacter> SoftEnemyClassToSpawn,
		int32 SpawnCount,
		const FVector& SpawnOrigin,
		float SpawnRadius
		);

	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemeisDelegate OnEnemiesSpawnedSuccess;		// 敌人生成成功委托

	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemeisDelegate OnEnemiesSpawnedFailed;		// 敌人生成失败委托

protected:
	
	
	// UGameplayTask interface
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	// ~UGameplayTask interface


private:
	FGameplayTag CachedEventTag;
	TSoftClassPtr<AArcaneEnemyCharacter> CachedSoftEnemyClassToSpawn;
	int32 CachedSpawnCount;
	FVector CachedSpawnOrigin;
	float CachedSpawnRadius;

	FDelegateHandle OnEnemiesSpawnedDelegateHandle;
	void OnEnemyClassLoaded();
	void OnGameplayEventReceived(const FGameplayEventData* GameplayEventData);
};
