// INVI_1998 All Rights Reserved.


#include "Widget/Options/OptionsDataInteractionHelper.h"

#include "FrontendSettings/FrontendGameUserSettings.h"


FOptionsDataInteractionHelper::FOptionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath)
	: CachedDynamicFunctionPath(InSetterOrGetterFuncPath)
{
	CachedWeakFrontendGameUserSettings = UFrontendGameUserSettings::Get();
}

FString FOptionsDataInteractionHelper::GetValueAsString() const
{
	FString OutStringValue;
	PropertyPathHelpers::GetPropertyValueAsString(CachedWeakFrontendGameUserSettings.Get(), CachedDynamicFunctionPath, OutStringValue);

	return OutStringValue;
}

void FOptionsDataInteractionHelper::SetValueFromString(const FString& InValue) const
{
	PropertyPathHelpers::SetPropertyValueFromString(
		CachedWeakFrontendGameUserSettings.Get(),
		CachedDynamicFunctionPath,
		InValue
	);
}
