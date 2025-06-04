// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneTypes/ArcaneEnumTypes.h"
#include "GameFramework/GameModeBase.h"
#include "ArcaneGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API AArcaneGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AArcaneGameModeBase();

	FORCEINLINE EArcaneGameDifficulty GetGameDifficulty() const { return GameDifficulty; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
	EArcaneGameDifficulty GameDifficulty = EArcaneGameDifficulty::Normal;	// 游戏难度设置
	
};
