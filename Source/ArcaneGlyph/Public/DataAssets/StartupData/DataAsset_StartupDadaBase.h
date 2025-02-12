// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartupDadaBase.generated.h"

class UArcaneGameplayAbility;
class UArcaneAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UDataAsset_StartupDadaBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UArcaneAbilitySystemComponent* InArcaneASC, int32 InApplyLevel = 1);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="StartupData")
	TArray<TSubclassOf<UArcaneGameplayAbility>> ActivateOnGivenAbilityList;		// 给定激活能力列表

	UPROPERTY(EditDefaultsOnly, Category="StartupData")
	TArray<TSubclassOf<UArcaneGameplayAbility>> ReactiveAbilityList;			// 反应能力列表(被动触发)

	void GrantAbilities(const TArray<TSubclassOf<UArcaneGameplayAbility>>& InAbilityList, UArcaneAbilitySystemComponent* InArcaneASC, int32 InApplyLevel = 1);
	
};
