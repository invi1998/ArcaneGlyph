// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameplayModMagnitudeCalculation.h"
#include "AbilitySystem/ArcaneAttributeSet.h"
#include "UMMC_PercentageHeal.generated.h"

struct ArcaneHealthCaptureStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);

	ArcaneHealthCaptureStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, CurrentHealth, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, MaxHealth, Source, true);
	}
};

static const ArcaneHealthCaptureStatics& GetHealthCaptureDef()
{
	static ArcaneHealthCaptureStatics HealthCaptureDef;
	return HealthCaptureDef;
}

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UMMC_PercentageHeal : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_PercentageHeal();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
};
