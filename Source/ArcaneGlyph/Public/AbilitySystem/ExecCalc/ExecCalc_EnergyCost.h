// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/ArcaneAttributeSet.h"
#include "ExecCalc_EnergyCost.generated.h"

struct ArcaneCostEnergyCaptureStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentEnergy);

	ArcaneCostEnergyCaptureStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, CurrentEnergy, Source, false);	// 捕获当前气力
	}
};

static const ArcaneCostEnergyCaptureStatics& GetArcaneCostEnergyCaptureStatics()
{
	static ArcaneCostEnergyCaptureStatics ArcaneCostEnergyCaptureDef;
	return ArcaneCostEnergyCaptureDef;
}

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UExecCalc_EnergyCost : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UExecCalc_EnergyCost();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
