// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendLoadingScreenSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UFrontendLoadingScreenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	// begin USubsystem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// end USubsystem Interface

	// begin FTickableGameObject Interface
	virtual UWorld* GetTickableGameObjectWorld() const override;	// 获取TickableGameObject所在的世界
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;	// 获取TickableGameObject的Tick类型
	virtual bool IsTickable() const override;	// 是否可被Tick
	virtual TStatId GetStatId() const override;	// 获取TickableGameObject的统计ID
	// end FTickableGameObject Interface

private:
	void OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName);
	void OnMapPostLoaded(UWorld* LoadedWorld);

	bool IsPreLoadingScreenActive() const;	// 预加载画面是否激活

	void TryUpdateLoadingScreen();
	bool bIsCurrentlyLoadingMap = false;	// 当前是否正在加载地图
	
};

