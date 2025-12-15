// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WaveDataTypes.generated.h"

// 1. 波次类型枚举
UENUM(BlueprintType)
enum class EWaveType : uint8
{
	Normal UMETA(DisplayName = "Normal Wave"),
	Boss UMETA(DisplayName = "Boss Wave"),
	Elite UMETA(DisplayName = "Elite Wave")
};

// 2. 单波次数据结构
USTRUCT(BlueprintType)
struct FWaveData
{
	GENERATED_BODY()

	/** 波次类型 (普通/Boss/精英) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	EWaveType WaveType = EWaveType::Normal;

	/** UI显示的波次名称 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	FText DisplayName;

	/** 在UI上代表该波次的图标资源引用 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TSoftObjectPtr<UTexture2D> WaveIcon;

	/** 该波次需要生成的敌人数量 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 EnemyCount = 10;
};

// 3. 难度配置数据资产
UCLASS(BlueprintType)
class UWaveDifficultyAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/** 难度的名称 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty")
	FText DifficultyName;

	/** 该难度下所有波次的序列 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficulty")
	TArray<FWaveData> Waves;
};

// 4. 波次状态枚举 (用于UI更新)
UENUM(BlueprintType)
enum class EWaveStatus : uint8
{
	Pending UMETA(DisplayName = "Pending"),
	Active UMETA(DisplayName = "Active"),
	Completed UMETA(DisplayName = "Completed")
};
