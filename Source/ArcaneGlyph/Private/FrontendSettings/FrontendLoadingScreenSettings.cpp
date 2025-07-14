// INVI_1998 All Rights Reserved.


#include "FrontendSettings/FrontendLoadingScreenSettings.h"

#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UFrontendLoadingScreenSettings::GetLoadingScreenWidgetClassChecked() const
{
	checkf(!SoftLoadingScreenWidgetClass.IsNull(), TEXT("Loading Screen Widget Class is not set in FrontendLoadingScreenSettings! Please set it in the project settings."));
	return SoftLoadingScreenWidgetClass.LoadSynchronous();
}
