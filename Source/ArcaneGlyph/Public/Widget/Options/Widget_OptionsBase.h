// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget/Widget_ActivatableBase.h"
#include "Widget_OptionsBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UWidget_OptionsBase : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	
	// begin UWidget interface
	virtual void NativeOnInitialized() override;
	// end UWidget interface

private:
	// 重设操作的行数据句柄
	UPROPERTY(EditDefaultsOnly, Category = "Frontend Options", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetActions;

	FUIActionBindingHandle ResetBindingHandle;	// 重设操作的绑定句柄

	// 重设操作按钮点击事件 （由于我们不是绑定到动态多播委托，只是简单的委托绑定，所以该绑定函数不需要UFUNCTION）
	void OnResetBoundActionsTriggered();
	void OnBackBoundActionsTriggered();
};
