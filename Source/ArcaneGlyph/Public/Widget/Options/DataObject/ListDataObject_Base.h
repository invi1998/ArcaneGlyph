// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ListDataObject_Base.generated.h"

// 该宏用于定义一个数据对象的属性访问器和属性赋值
#define LIST_DATA_ACCESSOR(DataObjectType, PropertyName) \
	FORCEINLINE DataObjectType Get##PropertyName() const { return PropertyName; } \
	void Set##PropertyName(DataObjectType In##PropertyName) { PropertyName = In##PropertyName; }

/**
 * 设置为Abstract（抽象）类型，这样在蓝图中就不能直接实例化该类，一些在蓝图需要设置的项目也不能直接检索到该类的实例，而只能通过子类进行设置
 */
UCLASS(Abstract)
class ARCANEGLYPH_API UListDataObject_Base : public UObject
{
	GENERATED_BODY()

public:
	LIST_DATA_ACCESSOR(FName, DataID)
	LIST_DATA_ACCESSOR(FText, DataDisplayName)
	LIST_DATA_ACCESSOR(FText, DataDescriptionRichText)
	LIST_DATA_ACCESSOR(FText, DisabledRichText)
	LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage)
	LIST_DATA_ACCESSOR(UListDataObject_Base*, ParentData)

	void InitDataObject();

	virtual TArray<UListDataObject_Base*> GetAllChildListData() const
	{
		return TArray<UListDataObject_Base*>();
	}

	virtual bool HasAnyChildListData() const
	{
		return false;
	}

protected:
	virtual void OnDataObjectInitialized()
	{
		// 在数据对象初始化后可以执行一些额外的逻辑
	}

private:
	FName DataID;	// 数据ID，用于唯一标识该数据对象
	FText DataDisplayName;	// 数据显示名称，用于在UI中显示该数据对象的名称
	FText DataDescriptionRichText;	// 数据描述富文本，用于在UI中显示该数据对象的详细描述
	FText DisabledRichText;		// 禁用状态的富文本描述，用于在UI中显示该数据对象的禁用状态描述
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;	// 描述图片的软引用，用于在UI中显示该数据对象的描述图片

	UPROPERTY(Transient)
	UListDataObject_Base* ParentData;		// 父数据对象，用于实现数据对象的继承关系
};
