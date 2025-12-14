// INVI_1998 All Rights Reserved.


#include "Game/ArcaneGameInstance.h"

#include "MoviePlayer.h"

TSoftObjectPtr<UWorld> UArcaneGameInstance::GetLevelWorldByTag(FGameplayTag LevelTag) const
{
	for (const FArcaneGameLevelSet& GameLevelSet : GameLevelSets)
	{
		if (!GameLevelSet.IsValid())
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

void UArcaneGameInstance::Init()
{
	Super::Init();
	
	// 地图加载前后回调（用于显示加载屏幕，非阻塞式的屏幕加载，在单独的线程执行）
	// FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::OnPreLoadMap);
	// FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnPostLoadMap);
}

void UArcaneGameInstance::OnPreLoadMap(const FString& InMapName)
{
	FLoadingScreenAttributes LoadingScreenAttributes;
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;		// 当加载完成时自动完成
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.0f;			// 最小加载屏幕显示时间
	LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
	
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}

void UArcaneGameInstance::OnPostLoadMap(UWorld* World)
{
	GetMoviePlayer()->StopMovie();
}
