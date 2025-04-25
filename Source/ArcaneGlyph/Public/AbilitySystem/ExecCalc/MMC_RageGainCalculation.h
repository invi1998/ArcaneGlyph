// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameplayModMagnitudeCalculation.h"
#include "AbilitySystem/ArcaneAttributeSet.h"
#include "MMC_RageGainCalculation.generated.h"

struct ArcaneRageGainCaptureStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentSpark);

	ArcaneRageGainCaptureStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, CurrentSpark, Source, false);	// 捕获当前火花
		
	}
};

static const ArcaneRageGainCaptureStatics& GetArcaneRageGainCaptureStatics()
{
	static ArcaneRageGainCaptureStatics ArcaneRageGainCaptureDef;
	return ArcaneRageGainCaptureDef;
}

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UMMC_RageGainCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_RageGainCalculation();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
