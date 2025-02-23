// INVI_1998 All Rights Reserved.


#include "ArcaneGlyph/Public/Controllers/ArcaneHeroController.h"


AArcaneHeroController::AArcaneHeroController()
{
	// IGenericTeamAgentInterface::SetGenericTeamId(0);
	// 这里和AIController中的设置团队 ID 的方式不同，AIController中是通过 AAIController::SetGenericTeamId 来设置团队 ID
	// 而因为我们这里没有继承自 AAIController，而 IGenericTeamAgentInterface::SetGenericTeamId 设置团队 ID 是一个空函数，所以这里设置团队 ID 是无效的
	// 所以我们在玩家控制器里新增一个成员变量来保存团队 ID FGenericTeamId HeroTeamID;	// 英雄团队 ID（玩家团队 ID）
	HeroTeamID = FGenericTeamId(0);
}

FGenericTeamId AArcaneHeroController::GetGenericTeamId() const
{
	return HeroTeamID;
}



