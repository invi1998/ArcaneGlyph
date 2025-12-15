// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "ArcaneTypes/WaveDataTypes.h"
#include "WaveNodeWidget.h"
#include "WaveProgressBarWidget.generated.h"

/**
 * 波数进度条主容器控件C++基类
 * 负责动态生成节点和管理整体进度
 * 蓝图子类应命名为 WBP_WaveProgressBar
 */
UCLASS()
class UWaveProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * 动态生成波数UI节点。
	 * 蓝图可调用，通常在游戏开始加载数据后调用。
	 */
	UFUNCTION(BlueprintCallable, Category = "Wave UI")
	void GenerateWavesUI(const TArray<FWaveData>& Waves);

	/**
	 * 更新整体进度和所有节点的状态。
	 * 蓝图可调用，由 GameState 或 GameMode 驱动。
	 * @param CurrentWaveIndex 当前正在进行的波次索引 (0-based)
	 * @param CurrentWaveKillProgress 当前波次击杀进度 (0.0 - 1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Wave UI")
	void UpdateProgress(int32 CurrentWaveIndex, float CurrentWaveKillProgress);

protected:
	// 使用 BindWidget 宏绑定蓝图中的控件
	// 蓝图中的控件名称必须与此处的变量名完全一致

	/** 节点容器 (HorizontalBox) - 用于动态添加节点和路径 */
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* NodeContainer;

	/** 整体进度条 (ProgressBar) - 用于显示总进度百分比 */
	UPROPERTY(meta = (BindWidget))
	UProgressBar* OverallProgressBar;

	/** 波次节点控件的蓝图类引用 (WBP_WaveNode) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave UI")
	TSubclassOf<UWaveNodeWidget> WaveNodeWidgetClass;

	/** 节点之间的连接路径图像控件的蓝图类引用 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave UI")
	TSubclassOf<UUserWidget> PathWidgetClass;

private:
	/** 存储所有已生成的节点控件实例 */
	UPROPERTY()
	TArray<UWaveNodeWidget*> WaveNodes;

	/** 总波次数量 */
	int32 TotalWaves = 0;
};