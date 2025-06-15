// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_Value.h"
#include "ListDataObject_String.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UListDataObject_String : public UListDataObject_Value
{
	GENERATED_BODY()

public:
	void AddDynamicOptionsString(const FString& InStringValue, const FText& InDisplayText);
	FORCEINLINE const FString& GetCurrentStringValue() const { return CurrentStringValue; }
	FORCEINLINE const FText& GetCurrentDisplayText() const { return CurrentDisplayText; }
	FORCEINLINE const TArray<FText>& GetAvailableOptionsTextArray() const { return AvailableOptionsTextArray; }
	FORCEINLINE const TArray<FString>& GetAvailableOptionsStringArray() const { return AvailableOptionsStringArray; }
	
protected:
	virtual void OnDataObjectInitialized() override;

	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);
	
	FString CurrentStringValue;	// 当前字符串值
	FText CurrentDisplayText;	// 当前显示文本
	
	TArray<FString> AvailableOptionsStringArray;	// 可用选项字符串数组
	TArray<FText> AvailableOptionsTextArray;		// 可用选项文本数组
};
