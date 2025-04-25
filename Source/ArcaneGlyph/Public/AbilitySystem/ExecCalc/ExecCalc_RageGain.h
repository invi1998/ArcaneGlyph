// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/ArcaneAttributeSet.h"
#include "ExecCalc_RageGain.generated.h"

struct ArcaneRageGainExecCaptureStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentSpark);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentRage);

	ArcaneRageGainExecCaptureStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, CurrentSpark, Source, false);	// 捕获当前火花
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, CurrentRage, Source, false);	// 捕获当前怒气
		
	}
};

static const ArcaneRageGainExecCaptureStatics& GetArcaneRageGainExecCaptureStatics()
{
	static ArcaneRageGainExecCaptureStatics ArcaneRageGainExecCaptureDef;
	return ArcaneRageGainExecCaptureDef;
}

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UExecCalc_RageGain : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_RageGain();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
