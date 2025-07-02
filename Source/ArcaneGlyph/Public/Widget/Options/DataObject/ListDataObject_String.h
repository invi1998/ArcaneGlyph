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

	virtual bool CanSetToForcedStringValue(const FString& InForcedStringValue) const override;
	virtual void OnSetToForcedStringValue(const FString& InForcedStringValue) override;
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

// String Enum
UCLASS()
class ARCANEGLYPH_API UListDataObject_StringEnum : public UListDataObject_String
{
	GENERATED_BODY()

public:
	template<typename EnumType>
	void AddEnumOption(EnumType EnumValue, const FText& DisplayText)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();		//	获取枚举类型的静态枚举对象
		const FString EnumString = StaticEnumOption->GetNameStringByValue(EnumValue);	// 获取枚举值对应的字符串表示
		AddDynamicOptionsString(EnumString, DisplayText);
	}

	template<typename EnumType>
	EnumType GetCurrentValueAsEnum() const
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		return static_cast<EnumType>(StaticEnumOption->GetValueByNameString(CurrentStringValue));
	}

	template<typename EnumType>
	void SetDefaultValueFromEnum(EnumType EnumValue)
	{
		if (const UEnum* StaticEnumOption = StaticEnum<EnumType>())
		{
			const FString EnumString = StaticEnumOption->GetNameStringByValue(EnumValue);
			SetDefaultValueFromString(EnumString);
		}
	}
	

private:
	
};

