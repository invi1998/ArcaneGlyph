// INVI_1998 All Rights Reserved.


#include "Widget/Options/Widget_OptionsBase.h"

#include "ArcaneDebugHelper.h"
#include "ICommonInputModule.h"
#include "Input/CommonUIInputTypes.h"

void UWidget_OptionsBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetActions.IsNull())
	{
		// 向绑定操作栏插入自定义绑定操作并保存绑定句柄
		ResetBindingHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				ResetActions,
				true, // 在操作栏中显示
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionsTriggered)
			)
		);
	}

	// 完成自定义绑定后，我们可以注册默认的返回操作
	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true, // 在操作栏中显示
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionsTriggered)
		)
	);
}

void UWidget_OptionsBase::OnResetBoundActionsTriggered()
{
	Debug::Print(TEXT("Reset Bound Actions Triggered!"), FColor::Green);
}

void UWidget_OptionsBase::OnBackBoundActionsTriggered()
{
	// 返回操作触发时，通常会关闭当前选项界面
	DeactivateWidget();
}
