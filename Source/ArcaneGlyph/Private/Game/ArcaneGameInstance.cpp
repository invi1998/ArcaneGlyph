// INVI_1998 All Rights Reserved.


#include "Game/ArcaneGameInstance.h"

TSoftObjectPtr<UWorld> UArcaneGameInstance::GetLevelWorldByTag(FGameplayTag LevelTag) const
{
	for (const FArcaneGameLevelSet& GameLevelSet : GameLevelSets)
	{
		if (!GameLevelSet.LevelWorld.IsValid())
		{
			continue; // 如果 LevelWorld 无效，则跳过
		}
		
		if (GameLevelSet.LevelTag == LevelTag)
		{
			return GameLevelSet.LevelWorld;
		}
	}

	return TSoftObjectPtr<UWorld>();
}
