// INVI_1998 All Rights Reserved.


#include "Widget/Options/DataObject/ListDataObject_KeyRemap.h"

#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"

void UListDataObject_KeyRemap::InitKeyRemapData(ECommonInputType InDesiredInputType,
                                                UEnhancedInputUserSettings* InEnhancedInputUserSettings,
                                                UEnhancedPlayerMappableKeyProfile* InPlayerMappableKeyProfile,
                                                const FPlayerKeyMapping& InPlayerKeyMapping, const FPlayerKeyMapping* InTriggerKeyMapping)
{
	CachedEnhancedInputUserSettings = InEnhancedInputUserSettings;
	CachedPlayerMappableKeyProfile = InPlayerMappableKeyProfile;
	CachedDesiredInputType = InDesiredInputType;
	CachedMappingName = InPlayerKeyMapping.GetMappingName();
	CachedMappableKeySlot = InPlayerKeyMapping.GetSlot();

	// 如果有触发键位映射，则缓存触发键位映射的名称和槽位
	if (InTriggerKeyMapping)
	{
		CachedTriggerName = InTriggerKeyMapping->GetMappingName();
		CachedTriggerKeySlot = InTriggerKeyMapping->GetSlot();
		HasTrigger = true;
	}
	else
	{
		HasTrigger = false;
	}
	
}

FSlateBrush UListDataObject_KeyRemap::GetIconFromCurrentKey() const
{
	check(CachedEnhancedInputUserSettings);
	
	FSlateBrush InputBrush;

	UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(CachedEnhancedInputUserSettings->GetLocalPlayer());

	check(CommonInputSubsystem);
	
	const bool bHasFoundBrush = UCommonInputPlatformSettings::Get()->TryGetInputBrush(
		InputBrush,
		GetOwningKeyMapping(CachedMappingName, CachedMappableKeySlot)->GetCurrentKey(),
		CachedDesiredInputType,
		CommonInputSubsystem->GetCurrentGamepadName()
	);

	return InputBrush;
	
}

FSlateBrush UListDataObject_KeyRemap::GetTriggerIconFromCurrentKey() const
{
	check(CachedEnhancedInputUserSettings);
	
	FSlateBrush InputBrush;

	UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(CachedEnhancedInputUserSettings->GetLocalPlayer());

	check(CommonInputSubsystem);
	
	const bool bHasFoundBrush = UCommonInputPlatformSettings::Get()->TryGetInputBrush(
		InputBrush,
		GetOwningKeyMapping(CachedTriggerName, CachedTriggerKeySlot)->GetCurrentKey(),
		CachedDesiredInputType,
		CommonInputSubsystem->GetCurrentGamepadName()
	);

	return InputBrush;
		
}

FPlayerKeyMapping* UListDataObject_KeyRemap::GetOwningKeyMapping(const FName& InMappingName, const EPlayerMappableKeySlot& InSlot) const
{
	check(CachedPlayerMappableKeyProfile);
	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = InMappingName;
	KeyArgs.Slot = InSlot;
	return CachedPlayerMappableKeyProfile->FindKeyMapping(KeyArgs);
}
