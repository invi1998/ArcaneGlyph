// INVI_1998 All Rights Reserved.


#include "Game/ArcaneSurvialGameModeBase.h"

void AArcaneSurvialGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	checkf(EnemyWaveSpawnerDataTable, TEXT("AArcaneSurvialGameModeBase::BeginPlay - EnemyWaveSpawnerDataTable is not set! Please set it in the editor or in the constructor."));

	SetCurrentSurvialState(EArcaneSurvialGameModeState::WaitSpawnNewWave);

	TotalWavesToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();
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
