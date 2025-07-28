// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/ArcaneAttributeSet.h"
#include "ExecCale_ManaCost.generated.h"

struct FArcaneCostManaCaptureStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentMana);

	FArcaneCostManaCaptureStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArcaneAttributeSet, CurrentMana, Source, false);
	}
};

static FArcaneCostManaCaptureStatics& GetArcaneCostManaCaptureStatics()
{
	static FArcaneCostManaCaptureStatics ArcaneCostManaCaptureStatics;
	return ArcaneCostManaCaptureStatics;
}

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UExecCale_ManaCost : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCale_ManaCost();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
