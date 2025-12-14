// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneTypes/ArcaneEnumTypes.h"
#include "GameFramework/SaveGame.h"
#include "ArcaneSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	EArcaneGameDifficulty LastSelectedGameDifficulty = EArcaneGameDifficulty::None;	// 上次选择的游戏难度
	
	UPROPERTY(BlueprintReadOnly)
	TSet<EArcaneGameDifficulty> UnlockedGameDifficulties;	// 已解锁的游戏难度
	
	
};
