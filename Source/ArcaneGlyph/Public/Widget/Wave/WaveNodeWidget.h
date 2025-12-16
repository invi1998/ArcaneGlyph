// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "ArcaneTypes/WaveDataTypes.h" // 包含数据结构
#include "WaveNodeWidget.generated.h"

class USizeBox;
class UScaleBox;
/**
 * 波数进度条中的单个波次节点控件C++基类
 * 蓝图子类应命名为 WBP_WaveNode
 */
UCLASS()
class UWaveNodeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * 初始化节点数据和外观。
	 * 蓝图可调用，用于在动态生成时传入数据。
	 */
	UFUNCTION(BlueprintCallable, Category = "Wave UI")
	void InitializeNode(const FWaveData& InWaveData, int32 WaveIndex);

	/**
	 * 更新节点状态（待进行/当前/已完成）。
	 * 蓝图可调用，用于接收游戏状态更新。
	 */
	UFUNCTION(BlueprintCallable, Category = "Wave UI")
	void UpdateStatus(EWaveStatus NewStatus);

protected:
	// 使用 BindWidget 宏绑定蓝图中的控件
	// 蓝图中的控件名称必须与此处的变量名完全一致
	
	UPROPERTY(meta = (BindWidget))
	USizeBox* BaseSizeBox;

	/** 波次图标 (Image) - 用于显示普通/Boss图标 */
	UPROPERTY(meta = (BindWidget))
	UImage* IconImage;

	/** 状态覆盖层 (Overlay) - 用于显示高亮、完成标记等 */
	UPROPERTY(meta = (BindWidget))
	UOverlay* StatusOverlay;

	/** 存储当前节点的数据 */
	UPROPERTY(BlueprintReadOnly, Category = "Wave Data")
	FWaveData WaveData;

	/** 存储当前节点的索引 */
	UPROPERTY(BlueprintReadOnly, Category = "Wave Data")
	int32 NodeIndex;
	
	UPROPERTY(EditDefaultsOnly, Category = "Wave UI")
	FLinearColor NormalWaveColor = FLinearColor::White; // 普通波次颜色
	
	UPROPERTY(EditDefaultsOnly, Category = "Wave UI")
	FLinearColor BossWaveColor = FLinearColor::Red; // Boss波次颜色
	
	UPROPERTY(EditDefaultsOnly, Category = "Wave UI")
	FLinearColor FinishedWaveColor = FLinearColor::Green; // 已完成波次颜色
	
	UPROPERTY(EditDefaultsOnly, Category = "Wave UI")
	int32 NormalIconSize = 14; // 普通波次图标大小
	
	UPROPERTY(EditDefaultsOnly, Category = "Wave UI")
	int32 BossIconSize = 20; // Boss波次图标大小

	/**
	 * 蓝图事件：当节点状态更新时触发，供蓝图实现具体的视觉效果。
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Wave UI")
	void OnStatusUpdated(EWaveStatus NewStatus);
};