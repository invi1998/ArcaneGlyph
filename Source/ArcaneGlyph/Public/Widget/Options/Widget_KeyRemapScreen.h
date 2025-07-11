// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget/Widget_ActivatableBase.h"
#include "Widget_KeyRemapScreen.generated.h"

class UCommonRichTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_KeyRemapScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	

private:
	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_KeyRemapMessage;	// 键位重映射提示文本块
};
