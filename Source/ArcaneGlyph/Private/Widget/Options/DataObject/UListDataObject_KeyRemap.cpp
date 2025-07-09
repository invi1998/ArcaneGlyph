// INVI_1998 All Rights Reserved.


#include "Widget/Options/DataObject/UListDataObject_KeyRemap.h"

void UUListDataObject_KeyRemap::InitKeyRemapData(ECommonInputType InDesiredInputType,
	UEnhancedInputUserSettings* InEnhancedInputUserSettings,
	UEnhancedPlayerMappableKeyProfile* InPlayerMappableKeyProfile, const FPlayerKeyMapping& InPlayerKeyMapping)
{
	CachedEnhancedInputUserSettings = InEnhancedInputUserSettings;
	CachedPlayerMappableKeyProfile = InPlayerMappableKeyProfile;
	CachedDesiredInputType = InDesiredInputType;
	CachedMappingName = InPlayerKeyMapping.GetMappingName();
	CachedMappableKeySlot = InPlayerKeyMapping.GetSlot();
}
