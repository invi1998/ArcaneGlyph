// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneHeroFreezeGameplayAbility.h"
#include "GenericTeamAgentInterface.h"
#include "ArcaneHeroStealthAbility.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMeleeAttackEventDelegate);

class APlayerPhantom;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneHeroStealthAbility : public UArcaneHeroGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Stealth Ability")
	TSubclassOf<APlayerPhantom> PhantomClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stealth Ability")
	APlayerPhantom* SpawnedPhantom = nullptr;
	
	UPROPERTY(BlueprintReadWrite, Category = "Stealth Ability")
	FTimerHandle PhantomDestroyTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stealth Ability")
	float PhantomLifetime = 15.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Stealth Ability")
	FGenericTeamId OriginalTeamID;
	
	UFUNCTION(BlueprintCallable, Category = "Stealth Ability")
	void RecoverCharacterTeamID();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Stealth Ability")
	void OnReceiveMeleeAttack();

	void OnReceiveMeleeAttackEvent(FGameplayTag GameplayTag, const FGameplayEventData* GameplayEventData);
	
	// 监听 Shared_Event_MeleeAttack_1 事件，即当玩家在隐身状态时，如果发动攻击并且攻击命中AI，则结束当前隐身状态
	UFUNCTION(BlueprintCallable, Category = "Stealth Ability")
	void ListhenMeleeAttackEventReceived();

	FDelegateHandle OnMeleeAttackEventDelegateHandle;

	static FOnMeleeAttackEventDelegate OnMeleeAttack; // 用于监听攻击事件

	// void OnMelleeAttackEventReceived
	
};
