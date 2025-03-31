// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcanePickUpBase.h"
#include "ArcaneStoneBase.generated.h"

class UGameplayEffect;
class UArcaneAbilitySystemComponent;

UCLASS()
class ARCANEGLYPH_API AArcaneStoneBase : public AArcanePickUpBase
{
	GENERATED_BODY()

public:
	virtual void Consume(UArcaneAbilitySystemComponent* AbilitySystemComponent, int32 InApplyLevel) override;

	UFUNCTION(BlueprintCallable)
	void ConsumeSoul(UArcaneAbilitySystemComponent* AbilitySystemComponent, int32 InApplyLevel);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StoneEffectClass;		// 石头效果

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On Soul Consume"))
	void BP_OnSoulConsume();
};
