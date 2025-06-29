// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OptionsDataRegistry.generated.h"

class UListDataObject_Base;
class UListDataObject_Collection;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UOptionsDataRegistry : public UObject
{
	GENERATED_BODY()

public:
	// 这个函数将由 选项设置页面 调用，紧接在我们的UOptionsDataRegistry对象创建之后
	void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);

	const TArray<UListDataObject_Collection*>& GetRegisteredOptionsTabCollections() const { return RegisteredOptionsTabCollections; }

	TArray<UListDataObject_Base*> GetListSourceItemBySelectedTabID(const FName& InTabID) const;

private:
	void FindChildListDataRecursively(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutChildListData) const;
	
	void InitGameplayCollectionTab();		// 游戏玩法
	void InitControlsCollectionTab();		// 控制
	void InitAudioCollectionTab();			// 音频
	void InitVideoCollectionTab();			// 视频

	UPROPERTY(Transient)
	TArray<UListDataObject_Collection*> RegisteredOptionsTabCollections;
	
};
