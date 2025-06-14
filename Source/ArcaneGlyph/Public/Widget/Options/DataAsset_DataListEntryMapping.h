// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_DataListEntryMapping.generated.h"

class UWidget_ListEntry_Base;
class UListDataObject_Base;
/**
 * 该数据资产用于存储数据列表项映射关系（即数据列表项与其对应的UI小部件类之间的映射关系）。
 * UListDataObject_Base 类型的子类（比如字符串类型，FName类型，布尔类型等他的子类类型）作为选项数据列表项，
 * UWidget_ListEntry_Base 类型的子类，就是对应的UI小部件类。（比如字符串类型在选项设置页面中对应的UI小部件类是 UWidget_ListEntry_String）
 * 说白了就是该映射用于指定CommonListView中每个类型数据列表项对应的UI小部件类。
 */
UCLASS()
class ARCANEGLYPH_API UDataAsset_DataListEntryMapping : public UDataAsset
{
	GENERATED_BODY()
	
public:
	TSubclassOf<UWidget_ListEntry_Base> FindEntryWidgetClassByDataObject(UListDataObject_Base* InDataObject) const;

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UListDataObject_Base>, TSubclassOf<UWidget_ListEntry_Base>> DataListEntryMapping;	// 数据列表项与其对应的UI小部件类之间的映射关系
};
