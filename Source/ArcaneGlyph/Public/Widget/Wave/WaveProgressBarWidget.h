// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Overlay.h" // 新增 Overlay 组件
#include "ArcaneTypes/WaveDataTypes.h"
#include "WaveNodeWidget.h"
#include "WaveProgressBarWidget.generated.h"

class UArcaneWidgetBase;
/**
 * 波数进度条主容器控件C++基类
 * 负责动态生成节点和管理整体进度
 */
UCLASS()
class UWaveProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Wave UI")
	void GenerateWavesUI(const TArray<FWaveData>& Waves);

	UFUNCTION(BlueprintCallable, Category = "Wave UI")
	void UpdateProgress(int32 CurrentWaveIndex, float CurrentWaveKillProgress);

protected:
	// 使用 BindWidget 宏绑定蓝图中的控件
	// 蓝图中的控件名称必须与此处的变量名完全一致

	/** 根容器 (Overlay) - 蓝图控件名必须是 RootOverlay */
	UPROPERTY(meta = (BindWidget))
	UOverlay* RootOverlay;

	/** 节点容器 (HorizontalBox) - 蓝图控件名必须是 NodeContainer */
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* NodeContainer;

	/** 整体进度条 (ProgressBar) - 蓝图控件名必须是 OverallProgressBar */
	UPROPERTY(meta = (BindWidget))
	UArcaneWidgetBase* OverallProgressBar;

	/** 波次节点控件的蓝图类引用 (WBP_WaveNode) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave UI")
	TSubclassOf<UWaveNodeWidget> WaveNodeWidgetClass;

private:
	UPROPERTY()
	TArray<UWaveNodeWidget*> WaveNodes;

	int32 TotalWaves = 0;
};