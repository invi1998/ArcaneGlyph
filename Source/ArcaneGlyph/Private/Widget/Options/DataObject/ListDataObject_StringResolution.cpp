// INVI_1998 All Rights Reserved.


#include "Widget/Options/DataObject/ListDataObject_StringResolution.h"

#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

void UListDataObject_StringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvaiableScreenResolutions;	// 支持的全屏分辨率列表

	// 获取支持的全屏分辨率
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvaiableScreenResolutions);

	// 然后将分辨率先进行排序
	AvaiableScreenResolutions.Sort([](const FIntPoint& A, const FIntPoint& B)-> bool
	{
		// return A.X * A.Y < B.X * B.Y; // 按照分辨率的像素总数进行排序
		// 等价于
		return A.SizeSquared() < B.SizeSquared(); // 按照分辨率的像素总数进行排序
	});

	for (const FIntPoint& Resolution : AvaiableScreenResolutions)
	{
		AddDynamicOptionsString(
			ResToValueString(Resolution), 
			ResToDisplayText(Resolution)
		);
	}

	MaximumAllowedResolution = ResToValueString(AvaiableScreenResolutions.Last()); // 设置最大允许分辨率为最后一个分辨率（即最大的分辨率）
	SetDefaultValueFromString(MaximumAllowedResolution);	// 设置默认值为最大允许分辨率
}

void UListDataObject_StringResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();
	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		// 如果无法从当前字符串值设置显示文本，则使用默认的屏幕分辨率
		CurrentDisplayText = ResToDisplayText(UFrontendGameUserSettings::Get()->GetScreenResolution());
	}
}

FString UListDataObject_StringResolution::ResToValueString(const FIntPoint& Resolution) const
{
	// Resolution value from DataDynamicGetter: (X=1920, Y=1080)
	// AvaiableOptionsStringArray: (X=1920, Y=1080)

	return FString::Printf(TEXT("(X=%i, Y=%i)"), Resolution.X, Resolution.Y);
}

FText UListDataObject_StringResolution::ResToDisplayText(const FIntPoint& Resolution) const
{
	return FText::FromString(FString::Printf(TEXT("%i x %i"), Resolution.X, Resolution.Y));
}
