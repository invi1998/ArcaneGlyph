// INVI_1998 All Rights Reserved.


#include "Widget/Component/FrontendTabListWidgetBase.h"

#include "Editor/WidgetCompilerLog.h"
#include "Widget/Component/FrontendCommonButtonBase.h"


void UFrontendTabListWidgetBase::RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName)
{
	// 只有调用了父类的RegisterTab，才能确保TabButtonEntryWidgetClass被正确设置，同时才能在蓝图中使用OnTabButtonCreation
	RegisterTab(InTabID, TabButtonEntryWidgetClass, nullptr);

	if (UFrontendCommonButtonBase* FoundButton = Cast<UFrontendCommonButtonBase>(GetTabButtonBaseByID(InTabID)))
	{
		FoundButton->SetButtenText(InTabDisplayName);
	}
}

#if WITH_EDITOR
void UFrontendTabListWidgetBase::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(
			TEXT("The variable TabButtonEntryWidgetClass has no valid entry specified. ") +
			GetClass()->GetName() + TEXT(" requires a valid widget class to be set for tab buttons.")
			));
	}
}
#endif
