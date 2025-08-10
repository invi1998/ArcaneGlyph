// INVI_1998 All Rights Reserved.


#include "AbilitySystem/Abilities/ArcaneHeroStealthAbility.h"

#include "Characters/PlayerPhantom.h"
#include "Controllers/ArcaneHeroController.h"

void UArcaneHeroStealthAbility::RecoverCharacterTeamID()
{
	if (AArcaneHeroController* HeroController = GetHeroControllerFromActorInfo())
	{
		HeroController->SetGenericTeamId(OriginalTeamID);
	}
}

void UArcaneHeroStealthAbility::SetOriginalTeamID()
{
	if (AArcaneHeroController* HeroController = GetHeroControllerFromActorInfo())
	{
		OriginalTeamID = HeroController->GetGenericTeamId();
	}
}
