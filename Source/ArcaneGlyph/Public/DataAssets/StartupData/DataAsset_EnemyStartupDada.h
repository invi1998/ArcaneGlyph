// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset_StartupDadaBase.h"
#include "DataAsset_EnemyStartupDada.generated.h"

class UArcaneEnemyGameplayAbility;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UDataAsset_EnemyStartupDada : public UDataAsset_StartupDadaBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UArcaneAbilitySystemComponent* InArcaneASC, int32 InApplyLevel = 1) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "StartupData")
	TArray<TSubclassOf<UArcaneEnemyGameplayAbility>> EnemyCombatAbilities;		// 敌人战斗能力列表
};
