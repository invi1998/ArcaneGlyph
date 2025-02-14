// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset_StartupDadaBase.h"
#include "ArcaneTypes/ArcaneStructTypes.h"
#include "UDataAsset_HeroStartupDada.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UUDataAsset_HeroStartupDada : public UDataAsset_StartupDadaBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UArcaneAbilitySystemComponent* InArcaneASC, int32 InApplyLevel = 1) override;

protected:

private:
	UPROPERTY(EditDefaultsOnly, Category = "HeroStartupData", meta = (TitleProperty = "InputTag"))
	TArray<FArcaneHeroAbilitySet> HeroStartupAbilitySets;		// 英雄启动能力集合
};
