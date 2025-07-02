// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_String.h"
#include "ListDataObject_StringResolution.generated.h"

/**
 * 屏幕分辨率列表数据对象类
 */
UCLASS()
class ARCANEGLYPH_API UListDataObject_StringResolution : public UListDataObject_String
{
	GENERATED_BODY()

public:
	void InitResolutionValues();
};
