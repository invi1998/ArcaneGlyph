// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneGameModeBase.h"
#include "ArcaneSurvialGameModeBase.generated.h"

UENUM(BlueprintType)
enum class EArcaneSurvialGameModeState : uint8
{
	WaitSpawnNewWave UMETA(DisplayName = "等待生成新波次"),
	SpawningNewWave UMETA(DisplayName = "生成新波次中"),
	WaveInProgress UMETA(DisplayName = "波次进行中"),
	WaveCompleted UMETA(DisplayName = "波次完成"),
	AllWavesCompleted UMETA(DisplayName = "所有波次完成"),
	PlayerDied UMETA(DisplayName = "玩家死亡"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvialGameModeStateChangedDelegate, EArcaneSurvialGameModeState, state);

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API AArcaneSurvialGameModeBase : public AArcaneGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;


private:
	void SetCurrentSurvialState(EArcaneSurvialGameModeState InState);
	
	UPROPERTY()
	EArcaneSurvialGameModeState CurrentSurvialGameModeState = EArcaneSurvialGameModeState::WaitSpawnNewWave;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSurvialGameModeStateChangedDelegate OnSurvialGameModeStateChanged;

};
