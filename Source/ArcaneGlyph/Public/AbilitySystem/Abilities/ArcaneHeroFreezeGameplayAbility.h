// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneHeroGameplayAbility.h"
#include "ArcaneHeroFreezeGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneHeroFreezeGameplayAbility : public UArcaneHeroGameplayAbility
{
	GENERATED_BODY()
	
public:
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Root")
	float BaseRootDuration = 10.0f; // 基础定身时间
    
	UPROPERTY(EditDefaultsOnly, Category = "Root")
	float ResilienceFactor = 0.1f; // 每点韧性减少的时间（秒）
    
	UPROPERTY(EditDefaultsOnly, Category = "Root")
	float DamageReductionPerHit = 0.5f; // 每次伤害减少的时间

	UPROPERTY(EditDefaultsOnly, Category = "Root")
	TSubclassOf<UGameplayEffect> FreezeEffectClass;

	UPROPERTY(BlueprintReadOnly)
	AActor* FreezeTargetActor;

	UFUNCTION(BlueprintCallable)
	void ApplyFreezeEffect(AActor* TargetActor);

	void OnDamageReceived(FGameplayTag DamageTag, const FGameplayEventData* Payload);
	
	void EndFreezeEffect();
    
private:
	FTimerHandle FreezeTimerHandle;
	FActiveGameplayEffectHandle ActiveFreezeEffectHandle;
	FDelegateHandle DamageEventHandle;
	
};
