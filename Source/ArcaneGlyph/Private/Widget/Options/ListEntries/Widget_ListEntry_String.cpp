// INVI_1998 All Rights Reserved.


#include "Widget/Options/ListEntries/Widget_ListEntry_String.h"

#include "ArcaneDebugHelper.h"
#include "CommonInputSubsystem.h"
#include "Widget/Component/FrontendCommonButtonBase.h"
#include "Widget/Component/FrontendCommonRotator.h"
#include "Widget/Options/DataObject/ListDataObject_String.h"

void UWidget_ListEntry_String::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 绑定上一个选项按钮的点击事件
	if (CommonButton_PreviousOption)
	{
		CommonButton_PreviousOption->OnClicked().AddUObject(this, &ThisClass::OnPreviousOptionClicked);
	}
	if (CommonButton_NextOption)
	{
		CommonButton_NextOption->OnClicked().AddUObject(this, &ThisClass::OnNextOptionClicked);
	}
	if (CommonRotator_AvailableOptions)
	{
		CommonRotator_AvailableOptions->OnClicked().AddUObject(this, &ThisClass::OnRotatorOptionClicked);

		// 绑定旋转器选项更改事件(CommonRotator_AvailableOptions会在选项更改时触发该事件，这样我们在使用手柄直接修改Rotator选项的时候就能像处理Pre和Next按钮一样处理选项切换了并保存结果）
		CommonRotator_AvailableOptions->OnRotatedEvent.AddUObject(this, &ThisClass::OnRotatorOptionChanged);
	}
}

void UWidget_ListEntry_String::OnOwningListDataObjectModified(UListDataObject_Base* InListDataObject, EOptionsListDataModifyReason InOptionsListDataModifyReason)
{
	if (OwningStringDataObject)
	{
		CommonRotator_AvailableOptions->SetSelectedOptionByText(OwningStringDataObject->GetCurrentDisplayText());
	}
}

void UWidget_ListEntry_String::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	OwningStringDataObject = CastChecked<UListDataObject_String>(InOwningListDataObject);

	CommonRotator_AvailableOptions->PopulateTextLabels(OwningStringDataObject->GetAvailableOptionsTextArray());

	// CommonUI 组件的Rotator会自动处理选项的切换和文本的更新，我们可以通过下面这些函数来实现
	// 但是这些函数有潜在的问题，很难准确追踪到正确选中项的索引位置，所以在项目里，我们不会直接使用这三个函数
	// 我们会使用UFrontendCommonRotator里自己二次封装的函数
	// CommonRotator_AvailableOptions->ShiftTextLeft();
	// CommonRotator_AvailableOptions->ShiftTextRight();
	// CommonRotator_AvailableOptions->SetSelectedItem();

	CommonRotator_AvailableOptions->SetSelectedOptionByText(OwningStringDataObject->GetCurrentDisplayText());
}

void UWidget_ListEntry_String::OnPreviousOptionClicked()
{
	if (OwningStringDataObject)
	{
		OwningStringDataObject->AdvanceToPreviousOption();
	}

	SelectThisEntryWidget();
}

void UWidget_ListEntry_String::OnNextOptionClicked()
{
	if (OwningStringDataObject)
	{
		OwningStringDataObject->AdvanceToNextOption();
	}

	SelectThisEntryWidget();
}

void UWidget_ListEntry_String::OnRotatorOptionClicked()
{
	SelectThisEntryWidget();
}

void UWidget_ListEntry_String::OnRotatorOptionChanged(int32 InSelectedIndex, bool bUserInitiated)
{
	if (OwningStringDataObject)
	{
		UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
		if (!CommonInputSubsystem || !bUserInitiated) return;
		
		if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
		{
			// 如果是手柄输入，我们需要手动处理选项的切换，依据当前选项的索引位置来切换到下一个或上一个选项
			OwningStringDataObject->OnRotatorInitiatedValueChanged(CommonRotator_AvailableOptions->GetSelectedText());
		}
	}
}
