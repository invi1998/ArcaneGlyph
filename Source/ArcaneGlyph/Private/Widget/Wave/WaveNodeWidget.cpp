// INVI_1998 All Rights Reserved.


#include "Widget/Wave/WaveNodeWidget.h"

#include "Components/SizeBox.h"

void UWaveNodeWidget::InitializeNode(const FWaveData& InWaveData, int32 WaveIndex)
{
	WaveData = InWaveData;
	NodeIndex = WaveIndex;

	// 可以在这里根据 WaveData.WaveType 设置 IconImage 的默认外观
	if (IconImage)
	{
		// 示例：根据波次类型设置不同的颜色或加载不同的图标
		if (WaveData.WaveType == EWaveType::Boss)
		{
			BaseSizeBox->SetWidthOverride(BossIconSize);
			BaseSizeBox->SetHeightOverride(BossIconSize);
			// 假设Boss波次使用红色
			IconImage->SetColorAndOpacity(BossWaveColor);
		}
		else
		{
			BaseSizeBox->SetWidthOverride(NormalIconSize);
			BaseSizeBox->SetHeightOverride(NormalIconSize);
			// 普通波次使用白色
			IconImage->SetColorAndOpacity(NormalWaveColor);
		}

		// 异步加载图标资源
		if (WaveData.WaveIcon.IsValid())
		{
			WaveData.WaveIcon.LoadSynchronous();
			IconImage->SetBrushFromTexture(WaveData.WaveIcon.Get());
		}
	}
}

void UWaveNodeWidget::UpdateStatus(EWaveStatus NewStatus)
{
	// 触发蓝图事件，让蓝图子类实现具体的视觉变化（如动画、高亮）
	OnStatusUpdated(NewStatus);
	
	if (NewStatus == EWaveStatus::Completed)
	{
		// 如果波次已完成
		if (IconImage)
		{
			// 将图标变为灰色以表示已完成
			IconImage->SetColorAndOpacity(FinishedWaveColor);
		}
	}
	
}
