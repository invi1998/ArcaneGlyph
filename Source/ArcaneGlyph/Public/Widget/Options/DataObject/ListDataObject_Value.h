// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_Base.h"
#include "ListDataObject_Value.generated.h"

class FOptionsDataInteractionHelper;
/**
 * 
 */
UCLASS(Abstract)
class ARCANEGLYPH_API UListDataObject_Value : public UListDataObject_Base
{
	GENERATED_BODY()

public:
	void SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDataDynamicGetter);
	void SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDataDynamicSetter);

	void SetDefaultValueFromString(const FString& InDefaultValue) { DefaultStringValue = InDefaultValue; }

	// **** Getter and Setter for DefaultStringValue **** //
	virtual bool HasDefaultValue() const override { return DefaultStringValue.IsSet(); }
	virtual bool CanResetToDefault() const override { return DefaultStringValue.IsSet(); }
	

protected:
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicGetter;
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicSetter;

	FString GetDefaultValueAsString() const
	{
		return DefaultStringValue.IsSet() ? DefaultStringValue.GetValue() : FString();
	}


private:

	TOptional<FString> DefaultStringValue;		// 默认字符串值，如果该值不为空，则表示该数据对象有默认值可供重置
	
};
