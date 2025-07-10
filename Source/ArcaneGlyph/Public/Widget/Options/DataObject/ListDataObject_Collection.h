// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_Base.h"
#include "ListDataObject_Collection.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UListDataObject_Collection : public UListDataObject_Base
{
	GENERATED_BODY()

public:
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const override;
	virtual bool HasAnyChildListData() const override;

	void AddChildListData(UListDataObject_Base* InChildListData);

private:
	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ChildListDataArray;
};

UCLASS()
class ARCANEGLYPH_API UListDataObject_CollectionInnerCategory : public UListDataObject_Collection
{
	GENERATED_BODY()
};
