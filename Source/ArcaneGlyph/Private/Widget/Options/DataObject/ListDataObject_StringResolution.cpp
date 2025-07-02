// INVI_1998 All Rights Reserved.


#include "Widget/Options/DataObject/ListDataObject_StringResolution.h"

#include "ArcaneDebugHelper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widget/Options/OptionsDataInteractionHelper.h"

void UListDataObject_StringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvaiableScreenResolutions;	// 支持的全屏分辨率列表

	// 获取支持的全屏分辨率
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvaiableScreenResolutions);

	for (const FIntPoint& Resolution : AvaiableScreenResolutions)
	{
		// Resolution value from DataDynamicGetter: (X=1920, Y=1080)
		FString ResolutionString = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
		AddDynamicOptionsString(ResolutionString, FText::FromString(ResolutionString));
	}
	
}

void UListDataObject_StringResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();
	if (DataDynamicGetter)
	{
		Debug::Print(TEXT("Resolution value from DataDynamicGetter: ") + DataDynamicGetter->GetValueAsString());
	}
}
