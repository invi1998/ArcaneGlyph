// INVI_1998 All Rights Reserved.


#include "Widget/Wave/WaveProgressBarWidget.h"

#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"

void UWaveProgressBarWidget::GenerateWavesUI(const TArray<FWaveData>& Waves)
{
	if (!NodeContainer || !WaveNodeWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("NodeContainer or WaveNodeWidgetClass is null in UWaveProgressBarWidget::GenerateWavesUI"));
		return;
	}

	// 1. 清理旧的节点
	NodeContainer->ClearChildren();
	WaveNodes.Empty();
	TotalWaves = Waves.Num();

	if (TotalWaves == 0)
	{
		return;
	}

	// 2. 动态生成节点和路径
	for (int32 i = 0; i < TotalWaves; ++i)
	{
		// A. 创建波次节点
		UWaveNodeWidget* NewNode = CreateWidget<UWaveNodeWidget>(this, WaveNodeWidgetClass);
		if (NewNode)
		{
			NewNode->InitializeNode(Waves[i], i);
			NodeContainer->AddChild(NewNode);
			WaveNodes.Add(NewNode);
		}

		// B. 在节点之间插入路径 (除了最后一个节点)
		if (i < TotalWaves - 1)
		{
			if (PathWidgetClass)
			{
				UUserWidget* PathWidget = CreateWidget<UUserWidget>(this, PathWidgetClass);
				if (PathWidget)
				{
					// 路径控件通常是一个 Image 或 SizeBox 包裹的 Image
					// 可以在蓝图 PathWidgetClass 中配置其外观
					NodeContainer->AddChild(PathWidget);
				}
			}
			else
			{
				// 如果没有 PathWidgetClass，可以插入一个简单的 Image 作为占位符
				UImage* PathImage = NewObject<UImage>(NodeContainer);
				if (PathImage)
				{
					// 设置路径的默认外观
					PathImage->SetColorAndOpacity(FLinearColor::Gray);
					UHorizontalBoxSlot* PathSlot = NodeContainer->AddChildToHorizontalBox(PathImage);
					PathSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill)); // 路径填充空间
				}
			}
		}
	}

	// 3. 初始化进度条
	if (OverallProgressBar)
	{
		OverallProgressBar->SetPercent(0.0f);
	}
}

void UWaveProgressBarWidget::UpdateProgress(int32 CurrentWaveIndex, float CurrentWaveKillProgress)
{
	if (TotalWaves == 0)
	{
		return;
	}

	// 1. 更新所有节点的状态
	for (int32 i = 0; i < TotalWaves; ++i)
	{
		if (WaveNodes.IsValidIndex(i))
		{
			EWaveStatus NewStatus = EWaveStatus::Pending;
			if (i < CurrentWaveIndex)
			{
				NewStatus = EWaveStatus::Completed;
			}
			else if (i == CurrentWaveIndex)
			{
				NewStatus = EWaveStatus::Active;
			}
			// 否则为 Pending

			WaveNodes[i]->UpdateStatus(NewStatus);
		}
	}

	// 2. 更新整体进度条
	if (OverallProgressBar)
	{
		// 计算整体进度：(已完成波次数 + 当前波次内部进度) / 总波次数量
		float OverallProgress = (float)CurrentWaveIndex + FMath::Clamp(CurrentWaveKillProgress, 0.0f, 1.0f);
		OverallProgress /= (float)TotalWaves;

		OverallProgressBar->SetPercent(OverallProgress);
	}
}
