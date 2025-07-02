// INVI_1998 All Rights Reserved.


#include "Widget/Options/OptionsDataRegistry.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneGameplayTags.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Widget/Options/OptionsDataInteractionHelper.h"
#include "Widget/Options/DataObject/ListDataObject_Collection.h"
#include "Widget/Options/DataObject/ListDataObject_Scalar.h"
#include "Widget/Options/DataObject/ListDataObject_String.h"
#include "Widget/Options/DataObject/ListDataObject_StringResolution.h"

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
		UListDataObject_StringBool* TutorialModeDataObject = NewObject<UListDataObject_StringBool>(GameplayCollectionDataObject, UListDataObject_StringBool::StaticClass());
		TutorialModeDataObject->SetDataID(FName("TutorialMode"));
		TutorialModeDataObject->SetDataDisplayName(FText::FromString(TEXT("教学模式")));
		TutorialModeDataObject->OverrideDisplayTrueText(FText::FromString(TEXT("开启"))); // 设置启用状态的显示文本
		TutorialModeDataObject->OverrideDisplayFalseText(FText::FromString(TEXT("关闭"))); // 设置禁用状态的显示文本
		TutorialModeDataObject->SetTrueAsDefaultValue(); // 设置默认值为启用状态
		TutorialModeDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameplayTutorialModeEnabled));
		TutorialModeDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameplayTutorialModeEnabled));
		TutorialModeDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改

		TutorialModeDataObject->SetDataDescriptionRichText(FText::FromString(TEXT("游戏教学模式核心设置：\n\n<Bold>难度级别</>\n- 休闲：敌人攻击频率降低60%，资源生成率翻倍\n- 标准：预设平衡体验（推荐新玩家）\n- 硬核：<Warning>永久关闭所有辅助功能</>\n\n<Bold>引导系统</>\n* 开启时提供：\n  - <Bold>交互高亮</>\n  - <Bold>路径指引</>\n  - <Bold>实时操作提示</>\n* <Warning>关闭后不可重新启用教程</>\n\n<Bold>辅助套件</>\n1. <Bold>智能存档</>（每5分钟自动存档）\n2. <Bold>谜题辅助</>（30秒后显示初级提示）\n3. <Bold>战斗辅助</>（危险动作触发0.75倍速）\n\n<Warning>重要警告</>\n* 硬核模式将锁定所有设置选项\n* 禁用智能存档可能导致进度丢失\n* 首次游玩建议开启全部引导功能\n\n<Bold>进阶提示</>\n→ 完成教程后进入设置界面\n→ 先关闭战斗辅助适应节奏\n→ 最后关闭路径指引挑战探索\n\n<Warning>配置须知</>\n变更设置后需重启当前关卡才能生效")));
		
		GameplayCollectionDataObject->AddChildListData(TutorialModeDataObject);
	}

	// 自动切换锁定目标
	{
		UListDataObject_StringBool* AutoTargetLockDataObject = NewObject<UListDataObject_StringBool>(GameplayCollectionDataObject, UListDataObject_StringBool::StaticClass());
		AutoTargetLockDataObject->SetDataID(FName("AutoTargetLock"));
		AutoTargetLockDataObject->SetDataDisplayName(FText::FromString(TEXT("自动切换锁定目标")));

		AutoTargetLockDataObject->SetTrueAsDefaultValue();
		AutoTargetLockDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameplayAutoTargetLock));
		AutoTargetLockDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameplayAutoTargetLock));
		AutoTargetLockDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改

		AutoTargetLockDataObject->SetDataDescriptionRichText(FText::FromString(TEXT("<Bold>自动切换锁定目标</>\n启用后自动追踪新进入视野的敌人：\n* 检测范围：<Number>15</>米锥形区域\n* 切换条件：目标进入视野<Number>0.5</>秒后\n* 优先级：距离最近>威胁值最高\n* 可调角度：<Number>30</>°~<Number>120</>°\n\n<Bold>工作机制</>\n* 目标死亡后<Number>0.3</>秒自动切换\n* 群战自动过滤血量<Number>10%</>以下目标\n* 镜头转动速度提升<Number>25%</>\n\n<Warning>使用注意</>\n* 开启后手动锁定需长按<Bold>LT键</>\n* 复杂地形可能意外切换目标\n* PvP场景禁用避免<Bold>锁定预测错误</>")));

		GameplayCollectionDataObject->AddChildListData(AutoTargetLockDataObject);
	}

	// 自动锁定攻击目标
	{
		UListDataObject_StringBool* AutoAttackTargetLockDataObject = NewObject<UListDataObject_StringBool>(GameplayCollectionDataObject, UListDataObject_StringBool::StaticClass());
		AutoAttackTargetLockDataObject->SetDataID(FName("AutoAttackTargetLock"));
		AutoAttackTargetLockDataObject->SetDataDisplayName(FText::FromString(TEXT("自动锁定攻击目标")));

		AutoAttackTargetLockDataObject->SetTrueAsDefaultValue();
		AutoAttackTargetLockDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameplayAutoAttackTargetLock));
		AutoAttackTargetLockDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameplayAutoAttackTargetLock));
		AutoAttackTargetLockDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改

		AutoAttackTargetLockDataObject->SetDataDescriptionRichText(FText::FromString(TEXT("<Bold>自动锁定攻击目标</>\n持续锁定当前目标直至脱离：\n* 视线丢失后维持<Number>3</>秒\n* 穿墙锁定：允许<Number>1.5</>米障碍物\n* 自动修正攻击方向偏移<Number>15</>°\n\n<Bold>高级设置</>\n* 锁定优先级：当前目标>仇恨最高\n* 切换冷却：<Number>1.2</>秒\n* Boss战锁定距离延长<Number>50%</>\n\n<Warning>平衡机制</>\n启用时：\n* 暴击率降低<Number>10%</>\n* 技能冷却增加<Number>0.5</>秒\n* 闪避消耗耐力增加<Number>20%</>\n\n<Warning>推荐配置</>\n* 单人游玩：建议开启\n* 团队副本：关闭避免OT\n* 竞技场：禁用保持操作自主性")));

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

	// 声音音量类别
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
			MasterVolumeDataObject->SetDefaultValueFromString(LexToString(1.f)); // 默认值为1.0（50%）
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
			MusicVolumeDataObject->SetDefaultValueFromString(LexToString(1.f)); // 默认值为1.0（50%）
			MusicVolumeDataObject->SetDisplayNumericType(ECommonNumericType::Percentage);	// 显示为百分比
			MusicVolumeDataObject->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal()); // 不显示小数点
			MusicVolumeDataObject->SetDataDescriptionRichText(FText::FromString(TEXT("<Bold>音乐音量控制</>\n独立调节背景音乐(BGM)强度，与主音量联动：\n* 基准音量为主音量的<Number>100%</>\n* 可通过此选项在<Number>-50%</>~<Number>+30%</>范围内偏移\n\n<Bold>特殊场景适配</>\n* 战斗状态：自动提升<Number>15%</>音量（可关闭）\n* 剧情过场：强制降低至<Number>80%</>避免台词覆盖\n\n<Warning>动态压缩警告</>\n当总音量超过<Number>95</>时：\n* 音乐将被压缩<Number>20%</>以保护听力\n* 压缩后最低保留<Number>40%</>原始音量\n\n<Bold>推荐设置</>\n→ 日常探索：保持<Number>0%</>偏移\n→ 音乐鉴赏：开启<Bold>独占模式</>（禁用动态压缩）\n→ 竞技对战：启用战斗增益\n\n<Warning>注意</>：开启独占模式可能触发音频过载")));
			
			// 设置动态获取器和设置器
			MusicVolumeDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMusicVolume));
			MusicVolumeDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMusicVolume));
			MusicVolumeDataObject->SetShouldApplyChangeImmediately(false); // 设置为不用立即应用更改

			VolumeCategoryCollection->AddChildListData(MusicVolumeDataObject);
		}

		// 特效音量
		{
			UListDataObject_Scalar* EffectsVolumeDataObject = NewObject<UListDataObject_Scalar>(VolumeCategoryCollection, UListDataObject_Scalar::StaticClass());
			EffectsVolumeDataObject->SetDataID(FName("EffectsVolume"));
			EffectsVolumeDataObject->SetDataDisplayName(FText::FromString(TEXT("特效音量")));
			EffectsVolumeDataObject->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			EffectsVolumeDataObject->SetOutputValueRange(TRange<float>(0.f, 2.f));
			EffectsVolumeDataObject->SetSliderStepSize(0.01f);
			EffectsVolumeDataObject->SetDefaultValueFromString(LexToString(1.0f)); // 默认值为1.0（50%）
			EffectsVolumeDataObject->SetDisplayNumericType(ECommonNumericType::Percentage); // 显示为百分比
			EffectsVolumeDataObject->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal()); // 不显示小数点
			EffectsVolumeDataObject->SetDataDescriptionRichText(FText::FromString(TEXT("<Bold>特效音量控制</>\n调节所有游戏内特效音频（如爆炸、技能等）的整体音量：\n* 范围：<Number>0%</>（静音）至<Number>100%</>（全开）\n* 默认值：<Number>70%</>\n\n<Bold>动态适配</>\n* 战斗状态下自动提升<Number>10%</>\n* 剧情过场时降低至<Number>50%</>\n\n<Warning>注意</>\n开启后可能导致某些高频特效过载，建议：\n1. 日常游玩保持<Number>60%</>\n2. 特效爱好者可尝试<Number>80%</>\n3. 竞技对战建议降低至<Number>40%</>\n\n<Bold>推荐设置</>\n→ 日常游玩：<Number>60%</>\n→ 特效爱好者：<Number>80%</>\n→ 竞技对战：<Number>40%</>\n\n<Warning>注意</>：开启后可能导致某些高频特效过载，建议：\n1. 日常游玩保持<Number>60%</>\n2. 特效爱好者可尝试<Number>80%</>\n3. 竞技对战建议降低至<Number>40%</>")));

			// 设置动态获取器和设置器
			EffectsVolumeDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSFXVolume));
			EffectsVolumeDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSFXVolume));
			EffectsVolumeDataObject->SetShouldApplyChangeImmediately(false); // 设置为不用立即应用更改

			VolumeCategoryCollection->AddChildListData(EffectsVolumeDataObject);

		}

		// 用户界面音量
		{
			UListDataObject_Scalar* UserInterfaceVolumeDataObject = NewObject<UListDataObject_Scalar>(VolumeCategoryCollection, UListDataObject_Scalar::StaticClass());
			UserInterfaceVolumeDataObject->SetDataID(FName("UserInterfaceVolume"));
			UserInterfaceVolumeDataObject->SetDataDisplayName(FText::FromString(TEXT("用户界面音量")));
			UserInterfaceVolumeDataObject->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			UserInterfaceVolumeDataObject->SetOutputValueRange(TRange<float>(0.f, 2.f));
			UserInterfaceVolumeDataObject->SetSliderStepSize(0.01f);
			UserInterfaceVolumeDataObject->SetDefaultValueFromString(LexToString(1.0f)); // 默认值为1.f（50%）
			UserInterfaceVolumeDataObject->SetDisplayNumericType(ECommonNumericType::Percentage); // 显示为百分比
			UserInterfaceVolumeDataObject->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal()); // 不显示小数点
			UserInterfaceVolumeDataObject->SetDataDescriptionRichText(FText::FromString(TEXT("<Bold>用户界面音量控制</>\n调节所有游戏内用户界面元素（如按钮、提示音等）的音量：\n* 范围：<Number>0%</>（静音）至<Number>100%</>（全开）\n* 默认值：<Number>50%</>\n\n<Bold>动态适配</>\n* 战斗状态下自动提升<Number>10%</>\n* 剧情过场时降低至<Number>30%</>\n\n<Warning>注意</>\n开启后可能导致某些高频特效过载，建议：\n1. 日常游玩保持<Number>40%</>\n2. 特效爱好者可尝试<Number>60%</>\n3. 竞技对战建议降低至<Number>30%</>\n\n<Bold>推荐设置</>\n→ 日常游玩：<Number>40%</>\n→ 特效爱好者：<Number>60%</>\n→ 竞技对战：<Number>30%</>\n\n<Warning>注意</>：开启后可能导致某些高频特效过载，建议：\n1. 日常游玩保持<Number>40%</>\n2. 特效爱好者可尝试<Number>60%</>\n3. 竞技对战建议降低至<Number>30%</>")));
			// 设置动态获取器和设置器
			UserInterfaceVolumeDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetUserInterfaceVolume));
			UserInterfaceVolumeDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetUserInterfaceVolume));
			UserInterfaceVolumeDataObject->SetShouldApplyChangeImmediately(false); // 设置为不用立即应用更改
			VolumeCategoryCollection->AddChildListData(UserInterfaceVolumeDataObject);
		}

		// 游戏内音乐音量
		{
			UListDataObject_Scalar* InGameMusicVolumeDataObject = NewObject<UListDataObject_Scalar>(VolumeCategoryCollection, UListDataObject_Scalar::StaticClass());
			InGameMusicVolumeDataObject->SetDataID(FName("InGameMusicVolume"));
			InGameMusicVolumeDataObject->SetDataDisplayName(FText::FromString(TEXT("游戏内音乐音量")));
			InGameMusicVolumeDataObject->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			InGameMusicVolumeDataObject->SetOutputValueRange(TRange<float>(0.f, 2.f));
			InGameMusicVolumeDataObject->SetSliderStepSize(0.01f);
			InGameMusicVolumeDataObject->SetDefaultValueFromString(LexToString(1.0f)); // 默认值为1.f（50%）
			InGameMusicVolumeDataObject->SetDisplayNumericType(ECommonNumericType::Percentage); // 显示为百分比
			InGameMusicVolumeDataObject->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal()); // 不显示小数点
			InGameMusicVolumeDataObject->SetDataDescriptionRichText(FText::FromString(TEXT("<Bold>游戏内音乐音量控制</>\n调节游戏内背景音乐的音量：\n* 范围：<Number>0%</>（静音）至<Number>100%</>（全开）\n* 默认值：<Number>60%</>\n\n<Bold>动态适配</>\n* 战斗状态下自动提升<Number>10%</>\n* 剧情过场时降低至<Number>50%</>\n\n<Warning>注意</>\n开启后可能导致某些高频特效过载，建议：\n1. 日常游玩保持<Number>50%</>\n2. 特效爱好者可尝试<Number>70%</>\n3. 竞技对战建议降低至<Number>40%</>\n\n<Bold>推荐设置</>\n→ 日常游玩：<Number>50%</>\n→ 特效爱好者：<Number>70%</>\n→ 竞技对战：<Number>40%</>\n\n<Warning>注意</>：开启后可能导致某些高频特效过载，建议：\n1. 日常游玩保持<Number>50%</>\n2. 特效爱好者可尝试<Number>70%</>\n3. 竞技对战建议降低至<Number>40%</>")));
			// 设置动态获取器和设置器
			InGameMusicVolumeDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetInGameMusicVolume));
			InGameMusicVolumeDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetInGameMusicVolume));
			InGameMusicVolumeDataObject->SetShouldApplyChangeImmediately(false); // 设置为不用立即应用更改
			VolumeCategoryCollection->AddChildListData(InGameMusicVolumeDataObject);
		}

		// 菜单音乐音量
		{
			UListDataObject_Scalar* MenuMusicVolumeDataObject = NewObject<UListDataObject_Scalar>(VolumeCategoryCollection, UListDataObject_Scalar::StaticClass());
			MenuMusicVolumeDataObject->SetDataID(FName("MenuMusicVolume"));
			MenuMusicVolumeDataObject->SetDataDisplayName(FText::FromString(TEXT("菜单音乐音量")));
			MenuMusicVolumeDataObject->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MenuMusicVolumeDataObject->SetOutputValueRange(TRange<float>(0.f, 2.f));
			MenuMusicVolumeDataObject->SetSliderStepSize(0.01f);
			MenuMusicVolumeDataObject->SetDefaultValueFromString(LexToString(1.0f)); // 默认值为1.f（50%）
			MenuMusicVolumeDataObject->SetDisplayNumericType(ECommonNumericType::Percentage); // 显示为百分比
			MenuMusicVolumeDataObject->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal()); // 不显示小数点
			MenuMusicVolumeDataObject->SetDataDescriptionRichText(FText::FromString(TEXT("<Bold>菜单音乐音量控制</>\n调节游戏主菜单背景音乐的音量：\n* 范围：<Number>0%</>（静音）至<Number>100%</>（全开）\n* 默认值：<Number>50%</>\n\n<Bold>动态适配</>\n* 战斗状态下自动提升<Number>10%</>\n* 剧情过场时降低至<Number>30%</>\n\n<Warning>注意</>\n开启后可能导致某些高频特效过载，建议：\n1. 日常游玩保持<Number>40%</>\n2. 特效爱好者可尝试<Number>60%</>\n3. 竞技对战建议降低至<Number>30%</>\n\n<Bold>推荐设置</>\n→ 日常游玩：<Number>40%</>\n→ 特效爱好者：<Number>60%</>\n→ 竞技对战：<Number>30%</>\n\n<Warning>注意</>：开启后可能导致某些高频特效过载，建议：\n1. 日常游玩保持<Number>40%</>\n2. 特效爱好者可尝试<Number>60%</>\n3. 竞技对战建议降低至<Number>30%</>")));
			// 设置动态获取器和设置器
			MenuMusicVolumeDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMenuMusicVolume));
			MenuMusicVolumeDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMenuMusicVolume));
			MenuMusicVolumeDataObject->SetShouldApplyChangeImmediately(false); // 设置为不用立即应用更改
			VolumeCategoryCollection->AddChildListData(MenuMusicVolumeDataObject);
		}
		
	}

	// 音频类别
	{
		UListDataObject_Collection* SoundCategoryCollection = NewObject<UListDataObject_Collection>(AudioCollectionDataObject);
		SoundCategoryCollection->SetDataID(FName("SoundCategoryCollection"));
		SoundCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("音频")));

		AudioCollectionDataObject->AddChildListData(SoundCategoryCollection);

		// 开启背景音乐
		{
			UListDataObject_StringBool* BackgroundMusicDataObject = NewObject<UListDataObject_StringBool>(SoundCategoryCollection, UListDataObject_StringBool::StaticClass());
			BackgroundMusicDataObject->SetDataID(FName("BackgroundMusic"));
			BackgroundMusicDataObject->SetDataDisplayName(FText::FromString(TEXT("开启背景音乐")));
			BackgroundMusicDataObject->OverrideDisplayTrueText(FText::FromString(TEXT("启用")));
			BackgroundMusicDataObject->OverrideDisplayFalseText(FText::FromString(TEXT("禁用")));
			BackgroundMusicDataObject->SetTrueAsDefaultValue();	// 设置默认值为启用
			BackgroundMusicDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowBackgroundAudio));
			BackgroundMusicDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowBackgroundAudio));
			BackgroundMusicDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改
			BackgroundMusicDataObject->SetDataDescriptionRichText(FText::FromString(TEXT("<Bold>背景音乐控制</>\n启用后游戏将播放背景音乐，禁用则静音所有背景音乐。\n\n<Bold>注意</>\n* 禁用后无法听到任何背景音乐\n* 建议新玩家开启以获得更好的沉浸感\n* 竞技玩家可选择禁用以减少干扰\n\n<Warning>警告</>\n禁用后无法恢复，请谨慎操作！")));

			AudioCollectionDataObject->AddChildListData(BackgroundMusicDataObject);
		}

		// 使用 HDR 音频模式
		{
			UListDataObject_StringBool* UseHDRAudioDataObject = NewObject<UListDataObject_StringBool>(SoundCategoryCollection, UListDataObject_StringBool::StaticClass());
			UseHDRAudioDataObject->SetDataID(FName("UseHDRAudio"));
			UseHDRAudioDataObject->SetDataDisplayName(FText::FromString(TEXT("使用 HDR 音频模式")));
			UseHDRAudioDataObject->OverrideDisplayTrueText(FText::FromString(TEXT("启用")));
			UseHDRAudioDataObject->OverrideDisplayFalseText(FText::FromString(TEXT("禁用")));
			UseHDRAudioDataObject->SetFalseAsDefaultValue();	// 设置默认值为禁用
			UseHDRAudioDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetUseHDRAudio));
			UseHDRAudioDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetUseHDRAudio));
			UseHDRAudioDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改
			UseHDRAudioDataObject->SetDataDescriptionRichText(FText::FromString(TEXT("<Bold>HDR音频模式</>\n启用高动态范围音频处理技术：\n* 动态范围扩展至<Number>144</>dB（标准模式<Number>96</>dB）\n* 细节分离度提升<Number>300%</>\n* 支持<Number>32</>位浮点音频处理\n\n<Bold>核心优势</>\n* 环境音效：可识别<Number>5</>米内细微声响\n* 空间定位：方向精度提升至<Number>5</>°\n* 动态响应：枪声等突发音压衰减快<Number>50%</>\n\n<Bold>硬件要求</>\n* 必需：支持<Bold>杜比全景声</>或<Bold>DTS:X</>\n* 推荐：<Number>7.1</>声道以上环绕系统\n* 耳机需开启<Bold>虚拟环绕</>功能\n\n<Warning>兼容性说明</>\n* 旧版DirectX可能造成音频撕裂\n* 启用时内存占用增加<Number>400</>MB\n* 部分蓝牙设备仅支持<Number>48</>kHz采样率\n\n<Bold>校准建议</>\n→ 首次使用运行<Bold>音频向导</>\n→ 安静环境下设置基准音量<Number>65</>\n→ 动态范围压缩保持<Number>30%</>\n\n<Warning>重要提示</>\n禁用后需重启游戏才能关闭HDR音频管线")));

			UseHDRAudioDataObject->SetDisabledRichText(FText::FromString(TEXT("<Warning>HDR音频模式已禁用</>\n\n<Bold>注意</>\n* 禁用后无法恢复HDR音频处理\n* 需重启游戏才能关闭HDR音频管线\n* 建议仅在高端音频设备上使用\n\n<Warning>警告</>：禁用后无法恢复，请谨慎操作！")));
			
			SoundCategoryCollection->AddChildListData(UseHDRAudioDataObject);
		}
		
	}

	
	RegisteredOptionsTabCollections.Add(AudioCollectionDataObject);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoCollectionDataObject = NewObject<UListDataObject_Collection>(this, UListDataObject_Collection::StaticClass());
	VideoCollectionDataObject->SetDataID(FName("VideoTabCollection"));
	VideoCollectionDataObject->SetDataDisplayName(FText::FromString(TEXT("显示")));

	UListDataObject_StringEnum* CreatedWindowModeDataObject = nullptr;
	
	// 屏幕显示类别
	{
		UListDataObject_Collection* DisplayCategoryCollection = NewObject<UListDataObject_Collection>(VideoCollectionDataObject);
		DisplayCategoryCollection->SetDataID(FName("DisplayCategoryCollection"));
		DisplayCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("显示设置")));
		
		VideoCollectionDataObject->AddChildListData(DisplayCategoryCollection);

		FOptionDataEditConditionDescriptor PackagedBuildOnlyCondition;	// 打包版本专用条件
		PackagedBuildOnlyCondition.SetEditConditionFunction(
			[]()->bool
			{
				const bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld;
				return !bIsInEditor; // 仅在打包版本中生效
			}
		);
		PackagedBuildOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Warning>打包版本专用</>\n<Warning>此选项仅在打包后的游戏中生效，编辑器模式下不可用。</>"));
		
		// 窗口模式
		{
			UListDataObject_StringEnum* WindowModeDataObject = NewObject<UListDataObject_StringEnum>(DisplayCategoryCollection, UListDataObject_StringEnum::StaticClass());
			WindowModeDataObject->SetDataID(FName("WindowMode"));
			WindowModeDataObject->SetDataDisplayName(FText::FromString(TEXT("窗口模式")));
			WindowModeDataObject->AddEnumOption<EWindowMode::Type>(EWindowMode::Fullscreen, FText::FromString(TEXT("全屏")));
			WindowModeDataObject->AddEnumOption<EWindowMode::Type>(EWindowMode::WindowedFullscreen, FText::FromString(TEXT("无边框窗口")));
			WindowModeDataObject->AddEnumOption<EWindowMode::Type>(EWindowMode::Windowed, FText::FromString(TEXT("窗口")));
			WindowModeDataObject->SetDefaultValueFromEnum<EWindowMode::Type>(EWindowMode::WindowedFullscreen); // 默认值为无边框窗口模式
			// 注意：这里的 EWindowMode 枚举类型因为使用的是 Unreal Engine 内置的 EWindowMode，而且该枚举已经提供了默认的Getter和Setter方法，所以我们可以直接使用 MAKE_OPTIONS_DATA_CONTROL 宏来绑定它们。
			// 而不需要像其他类型那样手动实现 Getter 和 Setter 方法。
			WindowModeDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowModeDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
			WindowModeDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改
			WindowModeDataObject->SetDataDescriptionRichText(FText::FromString(TEXT("<Bold>窗口模式</>\n调整游戏画面显示方式：\n* <Bold>全屏</>：独占显示输出（推荐竞技玩家）\n* <Bold>无边框窗口</>：无缝切换应用（默认选择）\n* <Bold>窗口</>：可自由调整尺寸（适合多屏操作）\n\n<Bold>性能影响</>\n* 全屏：帧率提升<Number>10-15%</>\n* 无边框：多屏渲染延迟<Number>3</>ms\n* 窗口：GPU利用率降低<Number>8%</>\n\n<Bold>分辨率联动</>\n* 全屏：强制使用显示器原生分辨率\n* 无边框：自动匹配桌面分辨率\n* 窗口：可自定义<Number>16:9</>/<Number>21:9</>比例\n\n<Warning>切换注意</>\n* 全屏切换可能造成<Number>1-2</>秒黑屏\n* 窗口模式禁用<Bold>G-Sync/FreeSync</>\n* 无边框模式需关闭<Bold>HDR</>防撕裂\n\n<Bold>多屏配置</>\n→ 主屏游戏：全屏模式\n→ 副屏操作：无边框窗口\n→ 窗口录制：固定<Number>1280×720</>尺寸\n\n<Warning>驱动要求</>\nNVIDIA/AMD显卡需更新至<Number>2023</>年后驱动版本")));

			// 编辑器状态下当前设置项不可用
			WindowModeDataObject->AddEditCondition(PackagedBuildOnlyCondition);

			CreatedWindowModeDataObject = WindowModeDataObject; // 保存创建的窗口模式数据对象
			DisplayCategoryCollection->AddChildListData(WindowModeDataObject);
			
		}

		// 屏幕分辨率
		{
			UListDataObject_StringResolution* ScreenResolutionDataObject = NewObject<UListDataObject_StringResolution>(DisplayCategoryCollection, UListDataObject_StringResolution::StaticClass());
			ScreenResolutionDataObject->SetDataID(FName("ScreenResolution"));
			ScreenResolutionDataObject->SetDataDisplayName(FText::FromString(TEXT("屏幕分辨率")));

			ScreenResolutionDataObject->SetDataDescriptionRichText(FText::FromString(TEXT("<Bold>屏幕分辨率</>\n调整游戏渲染像素数量：\n* 标准比例：<Number>16:9</>（<Number>1920×1080</>/<Number>2560×1440</>）\n* 超宽屏比例：<Number>21:9</>（<Number>2560×1080</>/<Number>3440×1440</>）\n* 专业比例：<Number>32:9</>（<Number>3840×1080</>）\n\n<Bold>性能影响</>\n每提升一档分辨率：\n* GPU负载增加<Number>35-40%</>\n* 显存占用增加<Number>1.5</>倍\n* 帧率下降约<Number>25%</>\n\n<Bold>显示技术</>\n* 原生分辨率：最佳清晰度（推荐）\n* DLSS/FSR：<Number>4K</>下性能提升<Number>70%</>\n* 动态分辨率：范围<Number>75-100%</>\n\n<Warning>配置警告</>\n* 超过显示器原生分辨率将启用虚拟超采样\n* <Number>4K</>分辨率需至少<Number>8GB</>显存\n* 宽屏模式可能造成UI拉伸\n\n<Bold>推荐设置</>\n→ 竞技玩家：<Number>1920×1080</>@<Number>144</>Hz\n→ 画质玩家：<Number>2560×1440</>@<Number>90</>Hz\n→ 电影体验：<Number>3440×1440</>带鱼屏\n\n<Warning>重要提示</>\n变更分辨率后需：\n1. 重新校准HUD界面比例\n2. 调整字体大小防止溢出\n3. 测试<Number>30</>秒确保帧率稳定")));

			ScreenResolutionDataObject->InitResolutionValues();

			// 同样，对于屏幕分辨率，我们可以直接使用 Unreal Engine 内置的 UListDataObject_StringResolution 类来处理分辨率的获取和设置。
			ScreenResolutionDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolutionDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolutionDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改

			// 编辑器状态下当前设置项不可用
			ScreenResolutionDataObject->AddEditCondition(PackagedBuildOnlyCondition);

			// 当窗口模式设置为无边框窗口时，屏幕分辨率选项将变为不可编辑状态
			FOptionDataEditConditionDescriptor WindowModeEditCondition;
			WindowModeEditCondition.SetEditConditionFunction(
				[CreatedWindowModeDataObject]() -> bool
				{
					return CreatedWindowModeDataObject->GetCurrentValueAsEnum<EWindowMode::Type>() != EWindowMode::WindowedFullscreen;
				}
			);
			WindowModeEditCondition.SetDisabledRichReason(TEXT("\n\n<Warning>无边框窗口模式下不可编辑</>\n<Warning>当前窗口模式为无边框窗口，屏幕分辨率选项将被禁用。</>"));
			WindowModeEditCondition.SetForcedStringValue(ScreenResolutionDataObject->GetMaxAllowedResolution()); // 强制设置为当前最大分辨率值
			ScreenResolutionDataObject->AddEditCondition(WindowModeEditCondition);

			// 添加依赖关系（窗口模式变更时自动更新屏幕分辨率的编辑状态）
			ScreenResolutionDataObject->AddEditDependencyData(CreatedWindowModeDataObject);

			DisplayCategoryCollection->AddChildListData(ScreenResolutionDataObject);
		}
	}

	// 图形设置
	{
		UListDataObject_Collection* GraphicsCategoryCollection = NewObject<UListDataObject_Collection>(VideoCollectionDataObject);
		GraphicsCategoryCollection->SetDataID(FName("GraphicsCategoryCollection"));
		GraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("图形设置")));
		VideoCollectionDataObject->AddChildListData(GraphicsCategoryCollection);

		// 亮度
		{
			UListDataObject_Scalar* BrightnessDataObject = NewObject<UListDataObject_Scalar>(GraphicsCategoryCollection, UListDataObject_Scalar::StaticClass());
			BrightnessDataObject->SetDataID(FName("Brightness"));
			BrightnessDataObject->SetDataDisplayName(FText::FromString(TEXT("亮度（Gama值）")));
			BrightnessDataObject->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			BrightnessDataObject->SetOutputValueRange(TRange<float>(1.7f, 2.7f));	// 虚幻引擎的默认亮度为2.2f，
			BrightnessDataObject->SetSliderStepSize(0.1f);
			BrightnessDataObject->SetDefaultValueFromString(LexToString(2.2f)); // 默认值为0.5（2.2f）
			BrightnessDataObject->SetDisplayNumericType(ECommonNumericType::Percentage); // 显示为百分比
			BrightnessDataObject->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal()); // 不显示小数点
			BrightnessDataObject->SetShouldApplyChangeImmediately(false); // 设置为立即应用更改
			BrightnessDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetDisplayGama));
			BrightnessDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetDisplayGama));
			BrightnessDataObject->SetDataDescriptionRichText(FText::FromString(TEXT("<Bold>亮度调整</>\n调节游戏画面的整体亮度：\n* 范围：<Number>0%</>（最暗）至<Number>100%</>（最亮）\n* 默认值：<Number>50%</>\n\n<Bold>使用建议</>\n* 日间场景：<Number>60%</>\n* 夜间场景：<Number>40%</>\n* 高对比度显示器：<Number>30%</>\n\n<Warning>注意</>\n过高的亮度可能导致视觉疲劳，建议定期休息！")));

			GraphicsCategoryCollection->AddChildListData(BrightnessDataObject);
		}
	}

	RegisteredOptionsTabCollections.Add(VideoCollectionDataObject);
}
