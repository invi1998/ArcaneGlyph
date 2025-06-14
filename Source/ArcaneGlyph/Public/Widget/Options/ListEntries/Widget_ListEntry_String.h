// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget_ListEntry_Base.h"
#include "Widget_ListEntry_String.generated.h"

class UFrontendCommonButtonBase;
class UFrontendCommonRotator;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_ListEntry_String : public UWidget_ListEntry_Base
{
	GENERATED_BODY()

private:
	// **** Bound Widgets **** //
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_Decrease;	// 减少按钮

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonRotator* CommonRotator_AvailableOptions;	// 可用选项旋转器（选项选择器）

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_Increase;	// 增加按钮

	// **** Bound Widgets **** //
};
