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

	void AdvanceToNextOption();
	void AdvanceToPreviousOption();
	void OnRotatorInitiatedValueChanged(const FText& InNewValue);
	
protected:
	// Begin UListDataObject_Base interface
	virtual void OnDataObjectInitialized() override;
	
	virtual bool CanResetToDefault() const override;
	virtual bool TryResetToDefault() override;
	// End UListDataObject_Base interface
	
	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);
	
	FString CurrentStringValue;	// 当前字符串值
	FText CurrentDisplayText;	// 当前显示文本
	
	TArray<FString> AvailableOptionsStringArray;	// 可用选项字符串数组
	TArray<FText> AvailableOptionsTextArray;		// 可用选项文本数组
};

// String Bool
UCLASS()
class ARCANEGLYPH_API UListDataObject_StringBool : public UListDataObject_String
{
	GENERATED_BODY()

public:
	// 覆盖Bool类型的显示文本(用于在UI中显示不同设置项的选项状态（比如某些是 启用/禁用，某些是 开/关）)
	void OverrideDisplayTrueText(const FText& InDisplayText);
	void OverrideDisplayFalseText(const FText& InDisplayText);
	void SetTrueAsDefaultValue();
	void SetFalseAsDefaultValue();

protected:
	// Begin UListDataObject_Base interface
	virtual void OnDataObjectInitialized() override;
	// End UListDataObject_Base interface

private:
	void TryInitializeBoolValue();
	
	const FString TrueString = TEXT("true");
	const FString FalseString = TEXT("false");
};

