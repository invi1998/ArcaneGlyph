// INVI_1998 All Rights Reserved.


#include "AbilitySystem/TargetActor/ArcaneTA_Phantom.h"

AArcaneTA_Phantom::AArcaneTA_Phantom()
{
}

void AArcaneTA_Phantom::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	IGenericTeamAgentInterface::SetGenericTeamId(TeamID);
}

ETeamAttitude::Type AArcaneTA_Phantom::GetTeamAttitudeTowards(const AActor& Other) const
{
	return IGenericTeamAgentInterface::GetTeamAttitudeTowards(Other);
}

FGenericTeamId AArcaneTA_Phantom::GetGenericTeamId() const
{
	return IGenericTeamAgentInterface::GetGenericTeamId();
}

void AArcaneTA_Phantom::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
}

void AArcaneTA_Phantom::CancelTargeting()
{
	Super::CancelTargeting();
}
