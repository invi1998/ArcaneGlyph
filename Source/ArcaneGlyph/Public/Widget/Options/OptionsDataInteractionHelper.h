// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"

class UFrontendGameUserSettings;
/**
 * 
 */
class ARCANEGLYPH_API FOptionsDataInteractionHelper
{
public:
	FOptionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath);

	FString GetValueAsString() const;
	void SetValueFromString(const FString& InValue);

private:
	FCachedPropertyPath CachedDynamicFunctionPath;	// 缓存的动态函数路径，用于快速访问动态函数
	TWeakObjectPtr<UFrontendGameUserSettings> CachedWeakFrontendGameUserSettings;	// 缓存的弱引用，指向前端游戏用户设置对象

};
