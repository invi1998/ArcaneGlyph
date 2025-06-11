// INVI_1998 All Rights Reserved.


#include "Widget/Component/FrontendTabListWidgetBase.h"

#include "Editor/WidgetCompilerLog.h"
#include "Widget/Component/FrontendCommonButtonBase.h"


void UFrontendTabListWidgetBase::RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName)
{
	
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
