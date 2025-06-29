// INVI_1998 All Rights Reserved.


#include "Widget/Options/OptionsDataRegistry.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneGameplayTags.h"
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
		TutorialModeDataObject->SetDefaultValueFromString(TEXT("Enabled")); // 设置默认值为启用
		TutorialModeDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameplayTutorialModeEnabled));
		TutorialModeDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameplayTutorialModeEnabled));
		TutorialModeDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改

		TutorialModeDataObject->SetDataDescriptionRichText(FText::FromString(TEXT("游戏教学模式核心设置：\n\n<Bold>难度级别</>\n- 休闲：敌人攻击频率降低60%，资源生成率翻倍\n- 标准：预设平衡体验（推荐新玩家）\n- 硬核：<Warning>永久关闭所有辅助功能</>\n\n<Bold>引导系统</>\n* 开启时提供：\n  - <Bold>交互高亮</>\n  - <Bold>路径指引</>\n  - <Bold>实时操作提示</>\n* <Warning>关闭后不可重新启用教程</>\n\n<Bold>辅助套件</>\n1. <Bold>智能存档</>（每5分钟自动存档）\n2. <Bold>谜题辅助</>（30秒后显示初级提示）\n3. <Bold>战斗辅助</>（危险动作触发0.75倍速）\n\n<Warning>重要警告</>\n* 硬核模式将锁定所有设置选项\n* 禁用智能存档可能导致进度丢失\n* 首次游玩建议开启全部引导功能\n\n<Bold>进阶提示</>\n→ 完成教程后进入设置界面\n→ 先关闭战斗辅助适应节奏\n→ 最后关闭路径指引挑战探索\n\n<Warning>配置须知</>\n变更设置后需重启当前关卡才能生效")));
		
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
		CharacterStatusInfoDataObject->SetSoftDescriptionImage(UArcaneBlueprintFunctionLibrary::GetOptionsSoftImageByTag(ArcaneGameplayTags::Frontend_Image_TestImage));

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

	// 声音类别
	{
		UListDataObject_Collection* VolumeCategoryCollection = NewObject<UListDataObject_Collection>(AudioCollectionDataObject);
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("音量")));

		AudioCollectionDataObject->AddChildListData(VolumeCategoryCollection);
		
	}

	RegisteredOptionsTabCollections.Add(AudioCollectionDataObject);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoCollectionDataObject = NewObject<UListDataObject_Collection>(this, UListDataObject_Collection::StaticClass());
	VideoCollectionDataObject->SetDataID(FName("VideoTabCollection"));
	VideoCollectionDataObject->SetDataDisplayName(FText::FromString(TEXT("显示")));

	RegisteredOptionsTabCollections.Add(VideoCollectionDataObject);
}
