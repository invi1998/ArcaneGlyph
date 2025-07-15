// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/GameInstance.h"
#include "ArcaneGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FArcaneGameLevelSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta=(Categories ="GameData.Level"))
	FGameplayTag LevelTag;	// 关卡标签，用于标识该关卡集

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> LevelWorld;	// 关卡世界的软引用，用于异步加载关卡

	bool IsValid() const
	{
		return LevelTag.IsValid() && !LevelWorld.IsNull();
	}
	
};

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta=(GameplayTagFilter ="GameData.Level"))
	TSoftObjectPtr<UWorld> GetLevelWorldByTag(FGameplayTag LevelTag) const;


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FArcaneGameLevelSet> GameLevelSets;

	
	
};
