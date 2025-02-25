// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Component/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChangedDelegate, float, NewPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwnnerGameplayTagChangedDelegate, FGameplayTag, NewGameplayTag);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARCANEGLYPH_API UPawnUIComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentHealthChanged;	// 当前生命值变化

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnOwnnerGameplayTagChangedDelegate OnHaveAttackGameplayTag;	// 拥有攻击标签（即Enemy进入攻击状态）

};
