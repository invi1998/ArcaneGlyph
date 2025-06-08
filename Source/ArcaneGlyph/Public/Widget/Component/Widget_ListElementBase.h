// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_ListElementBase.generated.h"

class UCommonTextBlock;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_ListElementBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	void SetElementText(const FText& InElementText);

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* CommonTextBlock_ElementText;	// 元素文本文本块（绑定的控件）

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ElementText;	// 元素文本内容
	
};
