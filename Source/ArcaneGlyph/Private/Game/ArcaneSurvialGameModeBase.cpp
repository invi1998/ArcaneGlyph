// INVI_1998 All Rights Reserved.


#include "Game/ArcaneSurvialGameModeBase.h"

#include "NavigationSystem.h"
#include "Characters/ArcaneEnemyCharacter.h"
#include "Engine/AssetManager.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

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
			CurrentSpawnedEnemyCounter += TrySpawnEnemy();
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

	PreLoadedEnemyClasses.Empty();

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

int32 AArcaneSurvialGameModeBase::TrySpawnEnemy()
{
	if (TargetPointsArray.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointsArray);
	}

	checkf(!TargetPointsArray.IsEmpty(), TEXT("AArcaneSurvialGameModeBase::TrySpawnEnemy - TargetPointsArray is empty! Please check the target points in the level(%s)."), *GetWorld()->GetName());

	uint32 EnemySpawnedThisTime = 0;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (const FArcaneEnemyWaveSpawnerInfo& SpawnerInfo : GetCurrentWaveEnemySpawnerTableRow()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull()) continue;

		const int32 NumToSpawn = FMath::RandRange(SpawnerInfo.MinPerSpawnCount, SpawnerInfo.MaxPerSpawnCount);

		UClass* LoadedEnemyClass = PreLoadedEnemyClasses.FindChecked(SpawnerInfo.SoftEnemyClassToSpawn);

		for (int32 i = 0; i < NumToSpawn; ++i)
		{
			const int32 RandomTargetPointIndex = FMath::RandRange(0, TargetPointsArray.Num() - 1);
			const FVector SpawnOrigin = TargetPointsArray[RandomTargetPointIndex]->GetActorLocation();
			const FRotator SpawnRotation = TargetPointsArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();

			FVector RandomLocation;
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, SpawnOrigin, RandomLocation, 400.f);

			RandomLocation += FVector(0.f, 0.f, 150.f); // Adjust height to avoid spawning on the ground

			if (AArcaneEnemyCharacter* SpawnArcaneEnemy = GetWorld()->SpawnActor<AArcaneEnemyCharacter>(LoadedEnemyClass, RandomLocation, SpawnRotation, SpawnParameters))
			{
				SpawnArcaneEnemy->OnDestroyed.AddUniqueDynamic(this, &AArcaneSurvialGameModeBase::OnEnemyDestroyed);

				EnemySpawnedThisTime++;
				TotalSpawnedEnemyThisWaveCounter++;
			}
			if (!ShouldKeepSpawningEnemies())
			{
				return EnemySpawnedThisTime;
			}
		}
		
	}
	
	return 0;
}

bool AArcaneSurvialGameModeBase::ShouldKeepSpawningEnemies() const
{
	return TotalSpawnedEnemyThisWaveCounter < GetCurrentWaveEnemySpawnerTableRow()->TotalEnemyToSpawnThisWave;
}

void AArcaneSurvialGameModeBase::OnEnemyDestroyed(AActor* DestroyedActor)
{
	// 当前角色死亡，继续生成
	CurrentSpawnedEnemyCounter--;
	if (ShouldKeepSpawningEnemies())
	{
		CurrentSpawnedEnemyCounter += TrySpawnEnemy();
	}
	else if (CurrentSpawnedEnemyCounter == 0)
	{
		// 当前波次的敌人全部死亡，进入下一个状态
		TotalSpawnedEnemyThisWaveCounter = 0;
		CurrentSpawnedEnemyCounter = 0;
		SetCurrentSurvialState(EArcaneSurvialGameModeState::WaveCompleted);
	}
}
