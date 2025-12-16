// INVI_1998 All Rights Reserved.


#include "Widget/Wave/WaveProgressBarWidget.h"

#include "Components/HorizontalBoxSlot.h"
#include "Widget/ArcaneWidgetBase.h"

void UWaveProgressBarWidget::GenerateWavesUI(const TArray<FWaveData>& Waves)
{
	if (!NodeContainer || !OverallProgressBar || !WaveNodeWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("NodeContainer, OverallProgressBar, or WaveNodeWidgetClass is null in UWaveProgressBarWidget::GenerateWavesUI. Check BindWidget names and class reference."));
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

	// 2. 动态生成节点
	for (int32 i = 0; i < TotalWaves; ++i)
	{
		// A. 创建波次节点
		UWaveNodeWidget* NewNode = CreateWidget<UWaveNodeWidget>(this, WaveNodeWidgetClass);
		if (NewNode)
		{
			NewNode->InitializeNode(Waves[i], i);
			NodeContainer->AddChild(NewNode);
			WaveNodes.Add(NewNode);

			// B. 设置节点在 HorizontalBox 中的布局规则
			UHorizontalBoxSlot* NodeSlot = Cast<UHorizontalBoxSlot>(NewNode->Slot);
			if (NodeSlot)
			{
				// 使用 Fill 规则，确保节点均匀分布在进度条的宽度上
				NodeSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				// 居中对齐，确保节点位于进度条的中心线上
				NodeSlot->SetHorizontalAlignment(HAlign_Right);
				NodeSlot->SetVerticalAlignment(VAlign_Center);
			}
		}
	}

	// 3. 初始化进度条
	if (OverallProgressBar)
	{
		OverallProgressBar->BP_UpdateProgressBar(0.0f);
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
				if (FMath::IsNearlyEqual(CurrentWaveKillProgress, 1.0f))
				{
					NewStatus = EWaveStatus::Completed;
				}
				else
				{
					NewStatus = EWaveStatus::Active;
				}
			}
			WaveNodes[i]->UpdateStatus(NewStatus);
		}
	}

	// 2. 更新整体进度条
	if (OverallProgressBar)
	{
		float OverallProgress = static_cast<float>(CurrentWaveIndex) + FMath::Clamp(CurrentWaveKillProgress, 0.0f, 1.0f);
		OverallProgress /= static_cast<float>(TotalWaves);

		OverallProgressBar->BP_UpdateProgressBar(OverallProgress);
	}
}