// INVI_1998 All Rights Reserved.


#include "Widget/Options/OptionsDataRegistry.h"

#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Widget/Options/OptionsDataInteractionHelper.h"
#include "Widget/Options/DataObject/ListDataObject_Collection.h"
#include "Widget/Options/DataObject/ListDataObject_String.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterName))

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

	// 创建一个选项数据交互帮助器，用于获取和设置游戏设置中的教学模式启用状态
	// const TSharedPtr<FOptionsDataInteractionHelper> ConstructedHelper = MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, GetCurrentGameplayTutorialModeEnabled));

	// 教学模式
	{
		UListDataObject_String* TutorialModeDataObject = NewObject<UListDataObject_String>(GameplayCollectionDataObject, UListDataObject_String::StaticClass());
		TutorialModeDataObject->SetDataID(FName("TutorialMode"));
		TutorialModeDataObject->SetDataDisplayName(FText::FromString(TEXT("教学模式")));
		TutorialModeDataObject->AddDynamicOptionsString(TEXT("Enabled"), FText::FromString(TEXT("启用")));
		TutorialModeDataObject->AddDynamicOptionsString(TEXT("Disabled"), FText::FromString(TEXT("禁用")));
		TutorialModeDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameplayTutorialModeEnabled));
		TutorialModeDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameplayTutorialModeEnabled));
		TutorialModeDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改
		
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
