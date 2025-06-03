// INVI_1998 All Rights Reserved.


#include "Game/ArcaneSurvialGameModeBase.h"

#include "Characters/ArcaneEnemyCharacter.h"
#include "Engine/AssetManager.h"

void AArcaneSurvialGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	checkf(EnemyWaveSpawnerDataTable, TEXT("AArcaneSurvialGameModeBase::BeginPlay - EnemyWaveSpawnerDataTable is not set! Please set it in the editor or in the constructor."));

	SetCurrentSurvialState(EArcaneSurvialGameModeState::WaitSpawnNewWave);

	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();

	PreLoadNextWaveEnemy();
}

void AArcaneSurvialGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentSurvialGameModeState == EArcaneSurvialGameModeState::WaitSpawnNewWave)
	{
		TimePassedSinceStart += DeltaSeconds;

		if (TimePassedSinceStart >= SpawnNewWaveWaitTime)
		{
			TimePassedSinceStart = 0.0f;

			SetCurrentSurvialState(EArcaneSurvialGameModeState::SpawningNewWave);
		}
	}
	if (CurrentSurvialGameModeState == EArcaneSurvialGameModeState::SpawningNewWave)
	{
		TimePassedSinceStart += DeltaSeconds;
		if (TimePassedSinceStart >= SpawnEnemyDelayTime)
		{
			TimePassedSinceStart = 0.0f;

			SetCurrentSurvialState(EArcaneSurvialGameModeState::WaveInProgress);
		}
	}
	if (CurrentSurvialGameModeState == EArcaneSurvialGameModeState::WaveCompleted)
	{
		TimePassedSinceStart += DeltaSeconds;

		if (TimePassedSinceStart >= WaveCompletedWaitTime)
		{
			TimePassedSinceStart = 0.0f;
			CurrentWave++;

			if (HasFinishedAllWaves())
			{
				SetCurrentSurvialState(EArcaneSurvialGameModeState::AllWavesCompleted);
			}
			else
			{
				SetCurrentSurvialState(EArcaneSurvialGameModeState::WaitSpawnNewWave);
				PreLoadNextWaveEnemy();
			}
		}
	}
}

void AArcaneSurvialGameModeBase::SetCurrentSurvialState(EArcaneSurvialGameModeState InState)
{
	CurrentSurvialGameModeState = InState;

	OnSurvialGameModeStateChanged.Broadcast(CurrentSurvialGameModeState);
}

bool AArcaneSurvialGameModeBase::HasFinishedAllWaves() const
{
	return CurrentWave > TotalWavesToSpawn;
}

void AArcaneSurvialGameModeBase::PreLoadNextWaveEnemy()
{
	if (HasFinishedAllWaves()) return;

	for (const FArcaneEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveEnemySpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;

		// 预加载敌人类
		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			SpawnerInfo.SoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda([SpawnerInfo, this]()
			{
				if (UClass* EnemyClass = SpawnerInfo.SoftEnemyClassToSpawn.Get())
				{
					PreLoadedEnemyClasses.Emplace(SpawnerInfo.SoftEnemyClassToSpawn, EnemyClass);
				}
			})
		);
	}
	
}

FArcaneEnemyWaveSpawnerTableRow* AArcaneSurvialGameModeBase::GetCurrentWaveEnemySpawnerTableRow() const
{
	const FName RowName = FName(*FString::Printf(TEXT("Wave%d"), CurrentWave));
	FArcaneEnemyWaveSpawnerTableRow* FoundRow = EnemyWaveSpawnerDataTable->FindRow<FArcaneEnemyWaveSpawnerTableRow>(RowName, TEXT("AArcaneSurvialGameModeBase::GetCurrentWaveEnemySpawnerTableRow - Failed to find row in EnemyWaveSpawnerDataTable!"));

	checkf(FoundRow, TEXT("AArcaneSurvialGameModeBase::GetCurrentWaveEnemySpawnerTableRow - FoundRow is null! Please check the data table and the row name(%s)."), *RowName.ToString());
	return FoundRow;
}
