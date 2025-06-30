// INVI_1998 All Rights Reserved.


#include "Widget/Options/OptionsDataRegistry.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneGameplayTags.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Widget/Options/OptionsDataInteractionHelper.h"
#include "Widget/Options/DataObject/ListDataObject_Collection.h"
#include "Widget/Options/DataObject/ListDataObject_Scalar.h"
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
	
	UListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPoint;
	TArray<UListDataObject_Base*> AllChildListItems;
	for (UListDataObject_Base* ChildDataObject : FoundTabCollection->GetAllChildListData())
	{
		if (ChildDataObject)
		{
			AllChildListItems.Add(ChildDataObject);
			if (ChildDataObject->HasAnyChildListData())
			{
				// 如果该子数据对象还有子数据对象，则递归获取所有子数据对象
				FindChildListDataRecursively(ChildDataObject, AllChildListItems);
			}
		}
	}

	return AllChildListItems;
}

void UOptionsDataRegistry::FindChildListDataRecursively(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData())
	{
		return; // 如果父数据对象为空或没有子数据对象，则直接返回
	}

	for (UListDataObject_Base* ChildDataObject : InParentData->GetAllChildListData())
	{
		if (ChildDataObject)
		{
			OutChildListData.Add(ChildDataObject);
			if (ChildDataObject->HasAnyChildListData())
			{
				// 如果该子数据对象还有子数据对象，则递归获取所有子数据对象
				FindChildListDataRecursively(ChildDataObject, OutChildListData);
			}
		}
	}
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

		// 主音量
		{
			UListDataObject_Scalar* MasterVolumeDataObject = NewObject<UListDataObject_Scalar>(VolumeCategoryCollection, UListDataObject_Scalar::StaticClass());
			MasterVolumeDataObject->SetDataID(FName("MasterVolume"));
			MasterVolumeDataObject->SetDataDisplayName(FText::FromString(TEXT("主音量")));
			MasterVolumeDataObject->SetDataDescriptionRichText(FText::FromString(TEXT("音频设置中心：\n<Bold>主音量控制</> - 通过滑块调整所有声音元素的基准音量（范围：<Number>0</>~<Number>100</>），默认值<Number>75</>。\n\n特殊联动机制：\n* 背景音乐音量上限为主音量的<Number>80%</>\n* 环境音效受<Bold>动态压缩</>影响（高音量时自动降低<Number>15%</>）\n* 语音聊天独立增益上限<Number>+20%</>\n\n<Warning>听力保护提示</>\n持续暴露在<Number>85</>分贝以上可能造成听力损伤，建议：\n1. 日常游玩保持主音量≤<Number>70</>\n2. 佩戴耳机时启用<Bold>音量限制器</>（强制锁定≤<Number>60</>）\n\n<Bold>校准指南</>\n→ 在安静环境中播放测试音效\n→ 调整至刚好清晰听到<Number>20</>分贝提示音\n→ 保存后重启游戏使设置全局生效\n\n<Warning>注意</>：超过<Number>90</>将触发高频保护（自动过滤<Number>16000</>Hz以上音频）")));
			MasterVolumeDataObject->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MasterVolumeDataObject->SetOutputValueRange(TRange<float>(0.f, 2.f));
			MasterVolumeDataObject->SetSliderStepSize(0.01f);
			MasterVolumeDataObject->SetDefaultValueFromString(LexToString(1.f)); // 默认值为1.0（100%）
			MasterVolumeDataObject->SetDisplayNumericType(ECommonNumericType::Percentage);		// 显示为百分比
			MasterVolumeDataObject->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());	// 不显示小数点

			// 设置动态获取器和设置器
			MasterVolumeDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMasterVolume));
			MasterVolumeDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMasterVolume));
			MasterVolumeDataObject->SetShouldApplyChangeImmediately(false); // 设置为不用立即应用更改

			VolumeCategoryCollection->AddChildListData(MasterVolumeDataObject);
		}

		// 音乐音量
		{
			UListDataObject_Scalar* MusicVolumeDataObject = NewObject<UListDataObject_Scalar>(VolumeCategoryCollection, UListDataObject_Scalar::StaticClass());
			MusicVolumeDataObject->SetDataID(FName("MusicVolume"));
			MusicVolumeDataObject->SetDataDisplayName(FText::FromString(TEXT("音乐音量")));
			MusicVolumeDataObject->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MusicVolumeDataObject->SetOutputValueRange(TRange<float>(0.f, 2.f));
			MusicVolumeDataObject->SetSliderStepSize(0.01f);
			MusicVolumeDataObject->SetDefaultValueFromString(LexToString(0.8f)); // 默认值为0.8（80%）
			MusicVolumeDataObject->SetDisplayNumericType(ECommonNumericType::Percentage);	// 显示为百分比
			MusicVolumeDataObject->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal()); // 不显示小数点
			MusicVolumeDataObject->SetDataDescriptionRichText(FText::FromString(TEXT("<Bold>音乐音量控制</>\n独立调节背景音乐(BGM)强度，与主音量联动：\n* 基准音量为主音量的<Number>100%</>\n* 可通过此选项在<Number>-50%</>~<Number>+30%</>范围内偏移\n\n<Bold>特殊场景适配</>\n* 战斗状态：自动提升<Number>15%</>音量（可关闭）\n* 剧情过场：强制降低至<Number>80%</>避免台词覆盖\n\n<Warning>动态压缩警告</>\n当总音量超过<Number>95</>时：\n* 音乐将被压缩<Number>20%</>以保护听力\n* 压缩后最低保留<Number>40%</>原始音量\n\n<Bold>推荐设置</>\n→ 日常探索：保持<Number>0%</>偏移\n→ 音乐鉴赏：开启<Bold>独占模式</>（禁用动态压缩）\n→ 竞技对战：启用战斗增益\n\n<Warning>注意</>：开启独占模式可能触发音频过载")));
			
			// 设置动态获取器和设置器
			MusicVolumeDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMusicVolume));
			MusicVolumeDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMusicVolume));
			MusicVolumeDataObject->SetShouldApplyChangeImmediately(false); // 设置为不用立即应用更改

			VolumeCategoryCollection->AddChildListData(MusicVolumeDataObject);
		}
		
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
