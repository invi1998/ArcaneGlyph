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

protected:
	TArray<FString> AvailableOptionsStringArray;	// 可用选项字符串数组
	TArray<FString> AvailableValuesStringArray;
};
