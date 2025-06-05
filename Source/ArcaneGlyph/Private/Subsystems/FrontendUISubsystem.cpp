// INVI_1998 All Rights Reserved.


#include "Subsystems/FrontendUISubsystem.h"

UFrontendUISubsystem* UFrontendUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UFrontendUISubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

bool UFrontendUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		// 如果是专用服务器实例，就查找是否有派生类，如果没有派生类，则不创建子系统
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	return false;
}

void UFrontendUISubsystem::RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InWidget)
{
	check(InWidget);
	CreatedPrimaryLayoutWidget = InWidget;
}
