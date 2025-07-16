// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Input/UIActionBindingHandle.h"
#include "Widget/ArcaneWidgetBase.h"
#include "ArcaneWidget_BindAction.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisabledNativeTick))
class ARCANEGLYPH_API UArcaneWidget_BindAction : public UArcaneWidgetBase
{
	GENERATED_BODY()

public:

protected:
	virtual void NativeOnInitialized() override;

private:
	// 技能操作的行数据句柄，用于绑定技能操作的输入事件
	UPROPERTY(EditDefaultsOnly, Category = "Bind Actions", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle SpellActions;

	FUIActionBindingHandle SpellActionBindingHandle;

	// 技能释放的InputAction
	UPROPERTY(EditDefaultsOnly, Category = "Bind Actions", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UInputAction> SpellAction;
	
};
