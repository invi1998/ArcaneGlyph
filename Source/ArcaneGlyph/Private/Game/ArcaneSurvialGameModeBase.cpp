// INVI_1998 All Rights Reserved.


#include "Game/ArcaneSurvialGameModeBase.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneDebugHelper.h"
#include "NavigationSystem.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "Characters/ArcaneEnemyCharacter.h"
#include "Engine/AssetManager.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/Wave/WaveProgressBarWidget.h"

void AArcaneSurvialGameModeBase::RegisterSpawnedEnemies(const TArray<AArcaneEnemyCharacter*> InEnemiesToRegister)
{
	for (AArcaneEnemyCharacter* Enemy : InEnemiesToRegister)
	{
		if (UArcaneAbilitySystemComponent* EnemyASC = Enemy->GetArcaneAbilitySystemComponent())
		{
			// Enemy->OnDestroyed.AddUniqueDynamic(this, &AArcaneSurvialGameModeBase::OnEnemyDestroyed);
			EnemyASC->OnActorDeathDelegate.AddUObject(this, &AArcaneSurvialGameModeBase::OnEnemyDestroyed);
			CurrentSpawnedEnemyCounter++;
		}
	}
}

void AArcaneSurvialGameModeBase::SetCurrentSurvialState(EArcaneSurvialGameModeState InState)
{
	CurrentSurvialGameModeState = InState;

	OnSurvialGameModeStateChanged.Broadcast(CurrentSurvialGameModeState);
}

void AArcaneSurvialGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	// 根据游戏难度选择对应的数据表
	UDataTable* SelectedEnemyWaveSpawnerDataTable = EnemyWaveSpawnerDataTable.FindRef(GameDifficulty);

	checkf(SelectedEnemyWaveSpawnerDataTable, TEXT("AArcaneSurvialGameModeBase::BeginPlay - EnemyWaveSpawnerDataTable is not set! Please set it in the editor or in the constructor."));

	SetCurrentSurvialState(EArcaneSurvialGameModeState::WaitGameStart);

	TotalWavesToSpawn = SelectedEnemyWaveSpawnerDataTable->GetRowNames().Num();

	PreLoadNextWaveEnemy();
	
	CreateWaveProgressBarWidget();
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

void AArcaneSurvialGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	EArcaneGameDifficulty SelectedDifficulty = EArcaneGameDifficulty::Easy;
	UArcaneBlueprintFunctionLibrary::TryLoadSavedGameDifficulty(SelectedDifficulty);
	
	GameDifficulty = SelectedDifficulty;
	
}

void AArcaneSurvialGameModeBase::CreateWaveProgressBarWidget()
{
	TArray<FWaveData> WaveData;
	if (!ConvertDataTableToWaveData(
		EnemyWaveSpawnerDataTable.FindRef(GameDifficulty),
		WaveData,
		FText::FromString(UEnum::GetValueAsString(GameDifficulty))
	))
	{
		UE_LOG(LogTemp, Error, TEXT("AArcaneSurvialGameModeBase::CreateWaveProgressBarWidget - Failed to convert DataTable to WaveData."));
		return;
	}
	
	if (WaveProgressBarWidgetClass)
	{
		WaveProgressBarWidget = CreateWidget<UWaveProgressBarWidget>(GetWorld(), WaveProgressBarWidgetClass);
		if (WaveProgressBarWidget)
		{
			WaveProgressBarWidget->AddToViewport();
			WaveProgressBarWidget->GenerateWavesUI(WaveData);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AArcaneSurvialGameModeBase::CreateWaveProgressBarWidget - Failed to create WaveProgressBarWidget instance."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AArcaneSurvialGameModeBase::CreateWaveProgressBarWidget - WaveProgressBarWidgetClass is not set."));
	}
	
}

bool AArcaneSurvialGameModeBase::ConvertDataTableToWaveData(UDataTable* DataTable, TArray<FWaveData>& OutWaveData, const FText& DifficultyName)
{
	if (!DataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("ConvertDataTableToWaveData: Input DataTable is null."));
		return false;
	}

	// 检查 DataTable 的行结构是否匹配
	if (DataTable->RowStruct != FArcaneEnemyWaveSpawnerTableRow::StaticStruct())
	{
		UE_LOG(LogTemp, Error, TEXT("ConvertDataTableToWaveData: DataTable RowStruct mismatch. Expected FArcaneEnemyWaveSpawnerTableRow."));
		return false;
	}

	OutWaveData.Empty();
	
	// 获取所有行名和行数据
	TArray<FArcaneEnemyWaveSpawnerTableRow*> RowDataArray;
	DataTable->GetAllRows<FArcaneEnemyWaveSpawnerTableRow>(TEXT("ConvertDataTableToWaveData"), RowDataArray);

	if (RowDataArray.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ConvertDataTableToWaveData: DataTable contains no rows."));
		return true; // 成功，但结果为空
	}

	// 遍历所有行并转换
	for (int32 i = 0; i < RowDataArray.Num(); ++i)
	{
		const FArcaneEnemyWaveSpawnerTableRow* Row = RowDataArray[i];
		if (Row)
		{
			FWaveData NewWave = ConvertRowToWaveData(*Row, i);
			OutWaveData.Add(NewWave);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("ConvertDataTableToWaveData: Successfully converted %d rows."), OutWaveData.Num());
	return true;
}

FWaveData AArcaneSurvialGameModeBase::ConvertRowToWaveData(const FArcaneEnemyWaveSpawnerTableRow& RowData, int32 WaveIndex)
{
	FWaveData NewWaveData;

	// 1. 确定波次类型和敌人数量
	NewWaveData.EnemyCount = RowData.TotalEnemyToSpawnThisWave;
	
	// 区分 Boss 波次：如果总共需要生成的敌人数量为 1，则认为是 Boss 波次
	if (NewWaveData.EnemyCount == 1)
	{
		NewWaveData.WaveType = EWaveType::Boss;
		NewWaveData.DisplayName = FText::FromString(FString::Printf(TEXT("最终首领")));
	}
	else
	{
		NewWaveData.WaveType = EWaveType::Normal;
		NewWaveData.DisplayName = FText::FromString(FString::Printf(TEXT("第 %d 波"), WaveIndex + 1));
	}

	// 2. 确定图标 (可选，这里留空，用户可以在蓝图中配置)
	// 如果需要根据 RowData.EnemyWaveSpawnerDefinitions 来确定图标，需要更复杂的逻辑。
	// 暂时保持 WaveIcon 为空，让蓝图子类或 DataAsset 默认配置。
	
	return NewWaveData;
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
		if (SpawnerInfo.SoftEnemyClassToSpawn.IsNull())
		{
			continue;	
		}

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
	UDataTable* SelectedEnemyWaveSpawnerDataTable = EnemyWaveSpawnerDataTable.FindRef(GameDifficulty);
	FArcaneEnemyWaveSpawnerTableRow* FoundRow = SelectedEnemyWaveSpawnerDataTable->FindRow<FArcaneEnemyWaveSpawnerTableRow>(RowName, TEXT("AArcaneSurvialGameModeBase::GetCurrentWaveEnemySpawnerTableRow - Failed to find row in EnemyWaveSpawnerDataTable!"));

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
				// SpawnArcaneEnemy->OnDestroyed.AddUniqueDynamic(this, &AArcaneSurvialGameModeBase::OnEnemyDestroyed);
				if (UArcaneAbilitySystemComponent* EnemyASC = SpawnArcaneEnemy->GetArcaneAbilitySystemComponent())
				{
					EnemyASC->OnActorDeathDelegate.AddUObject(this, &AArcaneSurvialGameModeBase::OnEnemyDestroyed);
				}
				
				EnemySpawnedThisTime++;
				TotalSpawnedEnemyThisWaveCounter++;
			}
			if (!ShouldKeepSpawningEnemies())
			{
				return EnemySpawnedThisTime;
			}
		}
		
	}
	
	return EnemySpawnedThisTime;
}

bool AArcaneSurvialGameModeBase::ShouldKeepSpawningEnemies() const
{
	return TotalSpawnedEnemyThisWaveCounter < GetCurrentWaveEnemySpawnerTableRow()->TotalEnemyToSpawnThisWave;
}

void AArcaneSurvialGameModeBase::OnEnemyDestroyed(AActor* DestroyedActor)
{
	if (UArcaneAbilitySystemComponent* EnemyASC = UArcaneBlueprintFunctionLibrary::NativeGetArcaneASCFromActor(DestroyedActor))
	{
		EnemyASC->OnActorDeathDelegate.RemoveAll(this);
	}
	
	// 当前角色死亡，继续生成
	CurrentSpawnedEnemyCounter--;
	if (ShouldKeepSpawningEnemies())
	{
		CurrentSpawnedEnemyCounter += TrySpawnEnemy();
	}
	else if (CurrentSpawnedEnemyCounter <= 0)
	{
		// 当前波次的敌人全部死亡，进入下一个状态
		TotalSpawnedEnemyThisWaveCounter = 0;
		CurrentSpawnedEnemyCounter = 0;
		SetCurrentSurvialState(EArcaneSurvialGameModeState::WaveCompleted);
	}
}
