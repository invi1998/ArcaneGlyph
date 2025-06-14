// INVI_1998 All Rights Reserved.


#include "Widget/Options/OptionsDataRegistry.h"

#include "Widget/Options/DataObject/ListDataObject_Collection.h"
#include "Widget/Options/DataObject/ListDataObject_String.h"

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitControlsCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetListSourceItemBySelectedTabID(const FName& InTabID) const
{
	UListDataObject_Collection* const * FoundTabCollectionPoint = RegisteredOptionsTabCollections.FindByPredicate(
		[InTabID](const UListDataObject_Collection* Collection)->bool
		{
			return Collection && Collection->GetDataID() == InTabID;
		}
	);

	checkf(FoundTabCollectionPoint, TEXT("UOptionsDataRegistry::GetListSourceItemBySelectedTabID - Could not find collection with ID: %s"), *InTabID.ToString());
	return (*FoundTabCollectionPoint)->GetAllChildListData();
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayCollectionDataObject = NewObject<UListDataObject_Collection>(this, UListDataObject_Collection::StaticClass());
	GameplayCollectionDataObject->SetDataID(FName("GameplayTabCollection"));
	GameplayCollectionDataObject->SetDataDisplayName(FText::FromString(TEXT("游戏")));

	// 教学模式
	{
		UListDataObject_String* TutorialModeDataObject = NewObject<UListDataObject_String>(GameplayCollectionDataObject, UListDataObject_String::StaticClass());
		TutorialModeDataObject->SetDataID(FName("TutorialMode"));
		TutorialModeDataObject->SetDataDisplayName(FText::FromString(TEXT("教学模式")));

		GameplayCollectionDataObject->AddChildListData(TutorialModeDataObject);
	}

	// 自动切换锁定目标
	{
		UListDataObject_String* AutoTargetLockDataObject = NewObject<UListDataObject_String>(GameplayCollectionDataObject, UListDataObject_String::StaticClass());
		AutoTargetLockDataObject->SetDataID(FName("AutoTargetLock"));
		AutoTargetLockDataObject->SetDataDisplayName(FText::FromString(TEXT("自动切换锁定目标")));

		GameplayCollectionDataObject->AddChildListData(AutoTargetLockDataObject);
	}

	// 自动锁定攻击目标
	{
		UListDataObject_String* AutoAttackTargetLockDataObject = NewObject<UListDataObject_String>(GameplayCollectionDataObject, UListDataObject_String::StaticClass());
		AutoAttackTargetLockDataObject->SetDataID(FName("AutoAttackTargetLock"));
		AutoAttackTargetLockDataObject->SetDataDisplayName(FText::FromString(TEXT("自动锁定攻击目标")));

		GameplayCollectionDataObject->AddChildListData(AutoAttackTargetLockDataObject);
	}

	// 角色状态信息
	{
		UListDataObject_String* CharacterStatusInfoDataObject = NewObject<UListDataObject_String>(GameplayCollectionDataObject, UListDataObject_String::StaticClass());
		CharacterStatusInfoDataObject->SetDataID(FName("CharacterStatusInfo"));
		CharacterStatusInfoDataObject->SetDataDisplayName(FText::FromString(TEXT("角色状态信息")));

		GameplayCollectionDataObject->AddChildListData(CharacterStatusInfoDataObject);
	}

	RegisteredOptionsTabCollections.Add(GameplayCollectionDataObject);
}

void UOptionsDataRegistry::InitControlsCollectionTab()
{
	UListDataObject_Collection* ControlsCollectionDataObject = NewObject<UListDataObject_Collection>(this, UListDataObject_Collection::StaticClass());
	ControlsCollectionDataObject->SetDataID(FName("ControlsTabCollection"));
	ControlsCollectionDataObject->SetDataDisplayName(FText::FromString(TEXT("控制器")));

	RegisteredOptionsTabCollections.Add(ControlsCollectionDataObject);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioCollectionDataObject = NewObject<UListDataObject_Collection>(this, UListDataObject_Collection::StaticClass());
	AudioCollectionDataObject->SetDataID(FName("AudioTabCollection"));
	AudioCollectionDataObject->SetDataDisplayName(FText::FromString(TEXT("声音")));

	RegisteredOptionsTabCollections.Add(AudioCollectionDataObject);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoCollectionDataObject = NewObject<UListDataObject_Collection>(this, UListDataObject_Collection::StaticClass());
	VideoCollectionDataObject->SetDataID(FName("VideoTabCollection"));
	VideoCollectionDataObject->SetDataDisplayName(FText::FromString(TEXT("显示")));

	RegisteredOptionsTabCollections.Add(VideoCollectionDataObject);
}
