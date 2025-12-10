// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneHeroGameplayAbility.h"
#include "ArcaneHeroSpawnPhantom.generated.h"

/**
 * 角色生成幻影能力
 */
UCLASS()
class ARCANEGLYPH_API UArcaneHeroSpawnPhantom : public UArcaneHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
	UArcaneHeroSpawnPhantom();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Target Actor")
	TSubclassOf<AActor> PhantomClass;	// 幻影类
	
	
};
