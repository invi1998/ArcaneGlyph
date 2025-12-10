// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "ArcaneTA_Phantom.generated.h"

/**
 * 幻影目标选择器
 */
UCLASS()
class ARCANEGLYPH_API AArcaneTA_Phantom : public AGameplayAbilityTargetActor, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	AArcaneTA_Phantom();
	
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void CancelTargeting() override;
	
private:
	
	
};
