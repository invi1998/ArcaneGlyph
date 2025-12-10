// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitSpawnPhantom.generated.h"

class AArcaneHeroController;
class AArcaneHeroCharacter;
class APlayerPhantom;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitSpawnPhantomDelegate, APlayerPhantom*, Phantom);

/**
 * 等待生成幻象
 */
UCLASS()
class ARCANEGLYPH_API UAbilityTask_WaitSpawnPhantom : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Arcane | AbilityTasks",
		meta = (DisplayName = "Wait Gameplay Event And Spawn Phantom",
				HidePin="OwningAbility",
				DefaultToSelf="OwningAbility",
				BlueprintInternalUseOnly = "true"))
	static UAbilityTask_WaitSpawnPhantom* WaitSpawnPhantom(
		UGameplayAbility* OwningAbility,
		FGameplayTag EventTag,
		AArcaneHeroCharacter* InOriginalCharacter,
		AArcaneHeroController* InHeroController,
		FGenericTeamId TargetTeamID,
		TSoftClassPtr<APlayerPhantom> SoftPhantomClassToSpawn,
		float InSpawnDuration
	);

	UPROPERTY(BlueprintAssignable)
	FWaitSpawnPhantomDelegate OnPhantomSpawnedSuccess;		// 幻象生成成功委托

	UPROPERTY(BlueprintAssignable)
	FWaitSpawnPhantomDelegate OnPhantomSpawnedFailed;		// 幻象生成失败委托

protected:

	// UGameplayTask interface
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	// ~UGameplayTask interface

private:
	FGameplayTag CachedEventTag;
	TSoftClassPtr<APlayerPhantom> SoftPhantomClassToSpawn;
	
	UPROPERTY()
	TObjectPtr<AArcaneHeroCharacter> OriginalCharacter;

	UPROPERTY()
	TObjectPtr<AArcaneHeroController> HeroController;

	FGenericTeamId TargetTeamID;
	
	float SpawnDuration;

	FDelegateHandle OnPhantomSpawnedDelegateHandle;
	void OnPhantomClassLoaded();
	void OnGameplayEventReceived(const FGameplayEventData* Payload);
	
};
