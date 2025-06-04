// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneGameModeBase.h"
#include "ArcaneSurvialGameModeBase.generated.h"

class AArcaneEnemyCharacter;

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

USTRUCT(BlueprintType)
struct FArcaneEnemyWaveSpawnerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AArcaneEnemyCharacter> SoftEnemyClassToSpawn;

	// 这个波次的敌人的最低数量
	UPROPERTY(EditAnywhere)
	int32 MinPerSpawnCount = 1;

	// 这个波次的敌人的最大数量
	UPROPERTY(EditAnywhere)
	int32 MaxPerSpawnCount = 3;
	
};

// 定义一个表格行结构体，用于存储敌人波次生成器的信息
USTRUCT(BlueprintType)
struct FArcaneEnemyWaveSpawnerTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TArray<FArcaneEnemyWaveSpawnerInfo> EnemyWaveSpawnerDefinitions;	// 敌人波次生成器定义

	UPROPERTY(EditAnywhere)
	int32 TotalEnemyToSpawnThisWave = 1;	// 这个波次总共需要生成的敌人数量
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvialGameModeStateChangedDelegate, EArcaneSurvialGameModeState, state);

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API AArcaneSurvialGameModeBase : public AArcaneGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void RegisterSpawnedEnemies(const TArray<AArcaneEnemyCharacter*> InEnemiesToRegister);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;


private:
	void SetCurrentSurvialState(EArcaneSurvialGameModeState InState);
	bool HasFinishedAllWaves() const;
	void PreLoadNextWaveEnemy();
	FArcaneEnemyWaveSpawnerTableRow* GetCurrentWaveEnemySpawnerTableRow() const;
	int32 TrySpawnEnemy();
	bool ShouldKeepSpawningEnemies() const;

	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);
	
	UPROPERTY()
	EArcaneSurvialGameModeState CurrentSurvialGameModeState = EArcaneSurvialGameModeState::WaitSpawnNewWave;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSurvialGameModeStateChangedDelegate OnSurvialGameModeStateChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta=(AllowPrivateAccess = "true"))
	UDataTable* EnemyWaveSpawnerDataTable;	// 敌人波次生成器数据表

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta=(AllowPrivateAccess = "true"))
	int32 TotalWavesToSpawn = 0;	// 总共需要生成的波次数量

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta=(AllowPrivateAccess = "true"))
	int32 CurrentWave = 1;		// 当前波次的索引

	UPROPERTY()
	int32 CurrentSpawnedEnemyCounter = 0;	// 当前已生成的敌人数量

	UPROPERTY()
	int32 TotalSpawnedEnemyThisWaveCounter = 0;	// 当前波次总共生成的敌人数量

	UPROPERTY()
	TArray<AActor*> TargetPointsArray;	// 目标点数组，用于存储敌人生成的目标点

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta=(AllowPrivateAccess = "true"))
	float SpawnNewWaveWaitTime = 5.0f;	// 生成新波次的等待时间

	UPROPERTY()
	float TimePassedSinceStart = 0.0f;	// 从开始到现在经过的时间

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta=(AllowPrivateAccess = "true"))
	float SpawnEnemyDelayTime = 3.f;	// 生成敌人的延迟时间

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta=(AllowPrivateAccess = "true"))
	float WaveCompletedWaitTime = 5.0f;	// 波次完成后的等待时间

	UPROPERTY()
	TMap<TSoftClassPtr<AArcaneEnemyCharacter>, UClass*> PreLoadedEnemyClasses;	// 预加载的敌人类映射表，用于存储已加载的敌人类，以避免重复加载
	
};
