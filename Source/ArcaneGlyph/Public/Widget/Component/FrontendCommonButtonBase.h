// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FrontendCommonButtonBase.generated.h"

class UCommonTextBlock;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UFrontendCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtenText(FText InButtonText);

private:
	virtual void NativeOnInitialized() override;
	
	//*** Bound Widgets (绑定的控件) ***//
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_ButtonText;	// 按钮文本块（可选绑定）
	// ** Bound Widgets (绑定的控件) ***//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDisplayText;	// 按钮文本

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	bool bUserUpperCaseButtonText = false;	// 是否将按钮文本转换为大写

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDescriptionText;			// 按钮描述文本
	
};
