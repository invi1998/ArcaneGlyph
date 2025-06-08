// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FrontendCommonButtonBase.h"
#include "FrontendCommonGridButtonBase.generated.h"

class UWidget_ListElementBase;
class UVerticalBox;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UFrontendCommonGridButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetCategoryText(FText InCategoryText);

	UFUNCTION(BlueprintCallable)
	void SetTitleText(FText InTitleText);

	UFUNCTION(BlueprintCallable)
	void SetSubTitleText(FText InSubTitleText);

	UFUNCTION(BlueprintCallable)
	void SetDescriptionElementList(TArray<FText> InDescriptionLists);

	UFUNCTION(BlueprintCallable)
	void ToggleGridButtonLock(bool bLock);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnCurrentTextStyleChanged() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	bool bGridButtonLocked = false;	// 是否锁定网格按钮
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	FText CategoryText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	FText TitleText;	// 标题文本

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	FText SubTitleText;		// 子标题文本

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWidget_ListElementBase> DescriptionElementWidgetClass;	// 子描述小部件类
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	TArray<FText> DescriptionLists;	// 子描述列表
	
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* GridButton_CategoryText;	// 类别文本块（可选绑定）

	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* GridButton_TitleText;	// 标题文本块（可选绑定）

	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* GridButton_SubTitleText;	// 子标题文本块（可选绑定）

	UPROPERTY(meta=(BindWidgetOptional))
	UVerticalBox* GridButton_SubDescriptionVerticalBox;		// 子描述垂直盒子（可选绑定）

	// Style
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCommonTextStyle> CategoryTextStyle;	// 类别文本样式

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCommonTextStyle> TitleTextStyle;		// 标题文本样式

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCommonTextStyle> SubTitleTextStyle;	// 子标题文本样式
	
};
