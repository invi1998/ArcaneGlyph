// INVI_1998 All Rights Reserved.


#include "Widget/Options/DataObject/ListDataObject_KeyRemap.h"

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
	return FSlateBrush();
}

FSlateBrush UListDataObject_KeyRemap::GetTriggerIconFromCurrentKey() const
{
	return FSlateBrush();
}
