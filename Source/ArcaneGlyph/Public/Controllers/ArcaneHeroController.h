// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "ArcaneHeroController.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API AArcaneHeroController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AArcaneHeroController();

	// IGenericTeamAgentInterface Begin
	// 获取团队 ID
	virtual FGenericTeamId GetGenericTeamId() const override;
	// ~IGenericTeamAgentInterface End

protected:

private:
	FGenericTeamId HeroTeamID;	// 英雄团队 ID（玩家团队 ID）
	
};
