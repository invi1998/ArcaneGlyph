// INVI_1998 All Rights Reserved.


#include "Widget/Options/OptionsDataRegistry.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneDebugHelper.h"
#include "ArcaneGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Internationalization/StringTableRegistry.h"
#include "Widget/Options/OptionsDataInteractionHelper.h"
#include "Widget/Options/DataObject/ListDataObject_Collection.h"
#include "Widget/Options/DataObject/ListDataObject_Scalar.h"
#include "Widget/Options/DataObject/ListDataObject_String.h"
#include "Widget/Options/DataObject/ListDataObject_StringResolution.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterName))

#define GET_DESCRIPTION_TEXT(DescriptionKey) \
	LOCTABLE("/Game/Blueprints/UI/StringTables/ST_OptionsScreenDescription.ST_OptionsScreenDescription", DescriptionKey)

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitControlsCollectionTab(InOwningLocalPlayer);
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

		const FText TutorialModeDescription = GET_DESCRIPTION_TEXT("TutorialModeDescKey");
		
		TutorialModeDataObject->SetDataDescriptionRichText(TutorialModeDescription);
		
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

		const FText AutoTargetLockDescription = GET_DESCRIPTION_TEXT("AutoTargetLockDescKey");
		AutoTargetLockDataObject->SetDataDescriptionRichText(AutoTargetLockDescription);
		
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

		const FText AutoAttackTargetLockDescription = GET_DESCRIPTION_TEXT("AutoAttackTargetLockDescKey");
		AutoAttackTargetLockDataObject->SetDataDescriptionRichText(AutoAttackTargetLockDescription);
		
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

void UOptionsDataRegistry::InitControlsCollectionTab(ULocalPlayer* InOwningLocalPlayer)
{
	UListDataObject_Collection* ControlsCollectionDataObject = NewObject<UListDataObject_Collection>(this, UListDataObject_Collection::StaticClass());
	ControlsCollectionDataObject->SetDataID(FName("ControlsTabCollection"));
	ControlsCollectionDataObject->SetDataDisplayName(FText::FromString(TEXT("控制器")));

	UEnhancedInputLocalPlayerSubsystem* EISubsystem = InOwningLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	check(EISubsystem);

	// 获取当前玩家的增强输入用户设置
	UEnhancedInputUserSettings* EnhancedUserSettings = EISubsystem->GetUserSettings();

	check(EnhancedUserSettings);
	
	// 键盘鼠标控制
	{
		UListDataObject_Collection* KeyboardMouseCategoryCollection = NewObject<UListDataObject_Collection>(ControlsCollectionDataObject, UListDataObject_Collection::StaticClass());
		KeyboardMouseCategoryCollection->SetDataID(FName("KeyboardMouseCategoryCollection"));
		KeyboardMouseCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("键盘鼠标控制")));

		ControlsCollectionDataObject->AddChildListData(KeyboardMouseCategoryCollection);

		// 键鼠输入
		{
			//  TMap<FGameplayTag, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>
			for (const TPair<FGameplayTag, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : EnhancedUserSettings->GetAllSavedKeyProfiles())
			{
				UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;
				check(MappableKeyProfile);

				for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
				{
					
					for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings)
					{
						Debug::Print(
							TEXT(" Mapping ID : ") + KeyMapping.GetMappingName().ToString() +
							TEXT(" Display Name : ") + KeyMapping.GetDisplayName().ToString() +
							TEXT(" Bound Key : ") + KeyMapping.GetCurrentKey().GetDisplayName().ToString()
						);
					}
					
				}
			}
		}
	}

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
			MasterVolumeDataObject->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MasterVolumeDataObject->SetOutputValueRange(TRange<float>(0.f, 2.f));
			MasterVolumeDataObject->SetSliderStepSize(0.01f);
			MasterVolumeDataObject->SetDefaultValueFromString(LexToString(1.f)); // 默认值为1.0（50%）
			MasterVolumeDataObject->SetDisplayNumericType(ECommonNumericType::Percentage);		// 显示为百分比
			MasterVolumeDataObject->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());	// 不显示小数点

			const FText MasterVolumeDescription = GET_DESCRIPTION_TEXT("MasterVolumeDescKey");
			MasterVolumeDataObject->SetDataDescriptionRichText(MasterVolumeDescription);

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

			const FText MusicVolumeDescription = GET_DESCRIPTION_TEXT("MusicVolumeDescKey");
			MusicVolumeDataObject->SetDataDescriptionRichText(MusicVolumeDescription);
			
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

			const FText EffectsVolumeDescription = GET_DESCRIPTION_TEXT("EffectsVolumeDescKey");
			EffectsVolumeDataObject->SetDataDescriptionRichText(EffectsVolumeDescription);
			
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

			const FText UserInterfaceVolumeDescription = GET_DESCRIPTION_TEXT("UserInterfaceVolumeDescKey");
			UserInterfaceVolumeDataObject->SetDataDescriptionRichText(UserInterfaceVolumeDescription);
			
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

			const FText InGameMusicVolumeDescription = GET_DESCRIPTION_TEXT("InGameMusicVolumeDescKey");
			InGameMusicVolumeDataObject->SetDataDescriptionRichText(InGameMusicVolumeDescription);
			
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

			const FText MenuMusicVolumeDescription = GET_DESCRIPTION_TEXT("MenuMusicVolumeDescKey");
			MenuMusicVolumeDataObject->SetDataDescriptionRichText(MenuMusicVolumeDescription);
			
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

			const FText BackgroundMusicDescription = GET_DESCRIPTION_TEXT("BackgroundMusicDescKey");
			BackgroundMusicDataObject->SetDataDescriptionRichText(BackgroundMusicDescription);
			
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

			const FText UseHDRAudioDescription = GET_DESCRIPTION_TEXT("UseHDRAudioDescKey");
			UseHDRAudioDataObject->SetDataDescriptionRichText(UseHDRAudioDescription);

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

			const FText WindowModeDescription = GET_DESCRIPTION_TEXT("WindowModeDescKey");
			WindowModeDataObject->SetDataDescriptionRichText(WindowModeDescription);

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

			const FText ScreenResolutionDescription = GET_DESCRIPTION_TEXT("ScreenResolutionDescKey");
			ScreenResolutionDataObject->SetDataDescriptionRichText(ScreenResolutionDescription);
			
			ScreenResolutionDataObject->InitResolutionValues();

			// 同样，对于屏幕分辨率，我们可以直接使用 Unreal Engine 内置的 UGameUserSettings 类来处理分辨率的获取和设置。
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

		UListDataObject_StringInteger* CreatedOverallQualityDataObject = nullptr;

		// 亮度
		{
			UListDataObject_Scalar* BrightnessDataObject = NewObject<UListDataObject_Scalar>(GraphicsCategoryCollection, UListDataObject_Scalar::StaticClass());
			BrightnessDataObject->SetDataID(FName("Brightness"));
			BrightnessDataObject->SetDataDisplayName(FText::FromString(TEXT("亮度（Gama值）")));
			BrightnessDataObject->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			BrightnessDataObject->SetOutputValueRange(TRange<float>(1.7f, 2.7f));	// 虚幻引擎的默认亮度为2.2f，
			BrightnessDataObject->SetSliderStepSize(0.01f);
			BrightnessDataObject->SetDefaultValueFromString(LexToString(2.2f)); // 默认值为0.5（2.2f）
			BrightnessDataObject->SetDisplayNumericType(ECommonNumericType::Percentage); // 显示为百分比
			BrightnessDataObject->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal()); // 不显示小数点
			BrightnessDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改
			BrightnessDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetDisplayGama));
			BrightnessDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetDisplayGama));

			const FText BrightnessDescription = GET_DESCRIPTION_TEXT("BrightnessDescKey");
			BrightnessDataObject->SetDataDescriptionRichText(BrightnessDescription);
			
			GraphicsCategoryCollection->AddChildListData(BrightnessDataObject);
		}

		// 画质等级
		{
			UListDataObject_StringInteger* QualityLevelDataObject = NewObject<UListDataObject_StringInteger>(GraphicsCategoryCollection, UListDataObject_StringInteger::StaticClass());
			QualityLevelDataObject->SetDataID(FName("QualityLevel"));
			QualityLevelDataObject->SetDataDisplayName(FText::FromString(TEXT("画质等级")));
			QualityLevelDataObject->AddIntegerOption(0, FText::FromString(TEXT("低")));
			QualityLevelDataObject->AddIntegerOption(1, FText::FromString(TEXT("中")));
			QualityLevelDataObject->AddIntegerOption(2, FText::FromString(TEXT("高")));
			QualityLevelDataObject->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			QualityLevelDataObject->AddIntegerOption(4, FText::FromString(TEXT("影视级")));
			QualityLevelDataObject->SetDefaultValueFromInteger(3); // 默认值为极高画质

			const FText QualityLevelDescription = GET_DESCRIPTION_TEXT("QualityLevelDescKey");
			QualityLevelDataObject->SetDataDescriptionRichText(QualityLevelDescription);
			
			// 同样，对于画质等级，我们可以直接使用 Unreal Engine 内置的 UGameUserSettings 类来处理画质等级的获取和设置。
			QualityLevelDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
			QualityLevelDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
			QualityLevelDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改

			CreatedOverallQualityDataObject = QualityLevelDataObject; // 保存创建的画质等级数据对象

			GraphicsCategoryCollection->AddChildListData(QualityLevelDataObject);
		}

		// 分辨率缩放等级
		{
			UListDataObject_Scalar* ResolutionScaleDataObject = NewObject<UListDataObject_Scalar>(GraphicsCategoryCollection, UListDataObject_Scalar::StaticClass());
			ResolutionScaleDataObject->SetDataID(FName("ResolutionScale"));
			ResolutionScaleDataObject->SetDataDisplayName(FText::FromString(TEXT("3D 分辨率缩放等级")));
			ResolutionScaleDataObject->SetDisplayValueRange(TRange<float>(0.f, 1.0f));
			ResolutionScaleDataObject->SetOutputValueRange(TRange<float>(0.f, 1.f)); // 输出范围为0%到100%
			ResolutionScaleDataObject->SetSliderStepSize(0.01f); // 步长为1%
			ResolutionScaleDataObject->SetDisplayNumericType(ECommonNumericType::Percentage); // 显示为百分比
			ResolutionScaleDataObject->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal()); // 不显示小数点
			ResolutionScaleDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改
			ResolutionScaleDataObject->SetDefaultValueFromString(LexToString(1.0f)); // 默认值为1.0（100%）

			const FText ResolutionScaleDescription = GET_DESCRIPTION_TEXT("ResolutionScaleDescKey");
			ResolutionScaleDataObject->SetDataDescriptionRichText(ResolutionScaleDescription);
			
			// 同样，对于分辨率缩放等级，我们可以直接使用 Unreal Engine 内置的 UGameUserSettings 类来处理分辨率缩放的获取和设置。
			ResolutionScaleDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
			ResolutionScaleDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));
			
			// 添加依赖
			ResolutionScaleDataObject->AddEditDependencyData(CreatedOverallQualityDataObject); // 依赖于画质等级设置
			CreatedOverallQualityDataObject->AddEditDependencyData(ResolutionScaleDataObject); // 画质等级设置依赖于分辨率缩放

			GraphicsCategoryCollection->AddChildListData(ResolutionScaleDataObject);
		}

		// 全局光照
		{
			UListDataObject_StringInteger* GlobalIlluminationDataObject = NewObject<UListDataObject_StringInteger>(GraphicsCategoryCollection, UListDataObject_StringInteger::StaticClass());
			GlobalIlluminationDataObject->SetDataID(FName("GlobalIllumination"));
			GlobalIlluminationDataObject->SetDataDisplayName(FText::FromString(TEXT("全局光照")));
			GlobalIlluminationDataObject->AddIntegerOption(0, FText::FromString(TEXT("低")));
			GlobalIlluminationDataObject->AddIntegerOption(1, FText::FromString(TEXT("中")));
			GlobalIlluminationDataObject->AddIntegerOption(2, FText::FromString(TEXT("高")));
			GlobalIlluminationDataObject->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			GlobalIlluminationDataObject->AddIntegerOption(4, FText::FromString(TEXT("影视级")));
			GlobalIlluminationDataObject->SetDefaultValueFromInteger(3); // 默认值为极高全局光照
			
			// 同样，对于全局光照，我们可以直接使用 Unreal Engine 内置的 UGameUserSettings 类来处理全局光照的获取和设置。
			GlobalIlluminationDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
			GlobalIlluminationDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));
			GlobalIlluminationDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改

			const FText GlobalIlluminationDescription = GET_DESCRIPTION_TEXT("GlobalIlluminationDescKey");
			GlobalIlluminationDataObject->SetDataDescriptionRichText(GlobalIlluminationDescription);

			// 添加依赖，整体画质变更时自动更新全局光照的编辑状态
			GlobalIlluminationDataObject->AddEditDependencyData(CreatedOverallQualityDataObject);

			// 同样，全局光照也能反向影响整体画质设置，所以我们也需要添加反向依赖
			CreatedOverallQualityDataObject->AddEditDependencyData(GlobalIlluminationDataObject);
			
			GraphicsCategoryCollection->AddChildListData(GlobalIlluminationDataObject);
			
		}

		// 阴影质量
		{
			UListDataObject_StringInteger* ShadowQualityDataObject = NewObject<UListDataObject_StringInteger>(GraphicsCategoryCollection, UListDataObject_StringInteger::StaticClass());
			ShadowQualityDataObject->SetDataID(FName("ShadowQuality"));
			ShadowQualityDataObject->SetDataDisplayName(FText::FromString(TEXT("阴影质量")));
			ShadowQualityDataObject->AddIntegerOption(0, FText::FromString(TEXT("低")));
			ShadowQualityDataObject->AddIntegerOption(1, FText::FromString(TEXT("中")));
			ShadowQualityDataObject->AddIntegerOption(2, FText::FromString(TEXT("高")));
			ShadowQualityDataObject->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			ShadowQualityDataObject->AddIntegerOption(4, FText::FromString(TEXT("影视级")));
			ShadowQualityDataObject->SetDefaultValueFromInteger(3); // 默认值为极高阴影质量
			
			// 同样，对于阴影质量，我们可以直接使用 Unreal Engine 内置的 UGameUserSettings 类来处理阴影质量的获取和设置。
			ShadowQualityDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
			ShadowQualityDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));
			ShadowQualityDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改

			const FText ShadowQualityDescription = GET_DESCRIPTION_TEXT("ShadowQualityDescKey");
			ShadowQualityDataObject->SetDataDescriptionRichText(ShadowQualityDescription);

			ShadowQualityDataObject->AddEditDependencyData(CreatedOverallQualityDataObject);
			CreatedOverallQualityDataObject->AddEditDependencyData(ShadowQualityDataObject);

			GraphicsCategoryCollection->AddChildListData(ShadowQualityDataObject);
		}

		// 抗锯齿
		{
			UListDataObject_StringInteger* AntiAliasingDataObject = NewObject<UListDataObject_StringInteger>(GraphicsCategoryCollection, UListDataObject_StringInteger::StaticClass());
			AntiAliasingDataObject->SetDataID(FName("AntiAliasing"));
			AntiAliasingDataObject->SetDataDisplayName(FText::FromString(TEXT("抗锯齿")));
			AntiAliasingDataObject->AddIntegerOption(0, FText::FromString(TEXT("低")));
			AntiAliasingDataObject->AddIntegerOption(1, FText::FromString(TEXT("中")));
			AntiAliasingDataObject->AddIntegerOption(2, FText::FromString(TEXT("高")));
			AntiAliasingDataObject->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			AntiAliasingDataObject->AddIntegerOption(4, FText::FromString(TEXT("影视级")));
			AntiAliasingDataObject->SetDefaultValueFromInteger(3); // 默认值为极高抗锯齿
			
			// 同样，对于抗锯齿，我们可以直接使用 Unreal Engine 内置的 UGameUserSettings 类来处理抗锯齿的获取和设置。
			AntiAliasingDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
			AntiAliasingDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality));
			AntiAliasingDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改

			const FText AntiAliasingDescription = GET_DESCRIPTION_TEXT("AntiAliasingDescKey");
			AntiAliasingDataObject->SetDataDescriptionRichText(AntiAliasingDescription);

			AntiAliasingDataObject->AddEditDependencyData(CreatedOverallQualityDataObject);
			CreatedOverallQualityDataObject->AddEditDependencyData(AntiAliasingDataObject);
			GraphicsCategoryCollection->AddChildListData(AntiAliasingDataObject);
		}

		// 视野距离
		{
			UListDataObject_StringInteger* ViewDistanceDataObject = NewObject<UListDataObject_StringInteger>(GraphicsCategoryCollection, UListDataObject_StringInteger::StaticClass());
			ViewDistanceDataObject->SetDataID(FName("ViewDistance"));
			ViewDistanceDataObject->SetDataDisplayName(FText::FromString(TEXT("视野距离")));
			ViewDistanceDataObject->AddIntegerOption(0, FText::FromString(TEXT("低")));
			ViewDistanceDataObject->AddIntegerOption(1, FText::FromString(TEXT("中")));
			ViewDistanceDataObject->AddIntegerOption(2, FText::FromString(TEXT("高")));
			ViewDistanceDataObject->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			ViewDistanceDataObject->AddIntegerOption(4, FText::FromString(TEXT("影视级")));
			ViewDistanceDataObject->SetDefaultValueFromInteger(3); // 默认值为极高视野距离
			
			// 同样，对于视野距离，我们可以直接使用 Unreal Engine 内置的 UGameUserSettings 类来处理视野距离的获取和设置。
			ViewDistanceDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
			ViewDistanceDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));
			ViewDistanceDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改

			const FText ViewDistanceDescription = GET_DESCRIPTION_TEXT("ViewDistanceDescKey");
			ViewDistanceDataObject->SetDataDescriptionRichText(ViewDistanceDescription);

			ViewDistanceDataObject->AddEditDependencyData(CreatedOverallQualityDataObject);
			CreatedOverallQualityDataObject->AddEditDependencyData(ViewDistanceDataObject);

			GraphicsCategoryCollection->AddChildListData(ViewDistanceDataObject);
			
		}

		// 纹理质量
		{
			UListDataObject_StringInteger* TextureQualityDataObject = NewObject<UListDataObject_StringInteger>(GraphicsCategoryCollection, UListDataObject_StringInteger::StaticClass());
			TextureQualityDataObject->SetDataID(FName("TextureQuality"));
			TextureQualityDataObject->SetDataDisplayName(FText::FromString(TEXT("纹理质量")));
			TextureQualityDataObject->AddIntegerOption(0, FText::FromString(TEXT("低")));
			TextureQualityDataObject->AddIntegerOption(1, FText::FromString(TEXT("中")));
			TextureQualityDataObject->AddIntegerOption(2, FText::FromString(TEXT("高")));
			TextureQualityDataObject->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			TextureQualityDataObject->AddIntegerOption(4, FText::FromString(TEXT("影视级")));
			TextureQualityDataObject->SetDefaultValueFromInteger(3); // 默认值为极高纹理质量
			
			// 同样，对于纹理质量，我们可以直接使用 Unreal Engine 内置的 UGameUserSettings 类来处理纹理质量的获取和设置。
			TextureQualityDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
			TextureQualityDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));
			TextureQualityDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改

			const FText TextureQualityDescription = GET_DESCRIPTION_TEXT("TextureQualityDescKey");
			TextureQualityDataObject->SetDataDescriptionRichText(TextureQualityDescription);

			TextureQualityDataObject->AddEditDependencyData(CreatedOverallQualityDataObject);
			CreatedOverallQualityDataObject->AddEditDependencyData(TextureQualityDataObject);

			GraphicsCategoryCollection->AddChildListData(TextureQualityDataObject);
		}

		// 视觉效果
		{
			UListDataObject_StringInteger* VisualEffectDataObject = NewObject<UListDataObject_StringInteger>(GraphicsCategoryCollection, UListDataObject_StringInteger::StaticClass());
			VisualEffectDataObject->SetDataID(FName("VisualEffect"));
			VisualEffectDataObject->SetDataDisplayName(FText::FromString(TEXT("视觉效果")));
			VisualEffectDataObject->AddIntegerOption(0, FText::FromString(TEXT("低")));
			VisualEffectDataObject->AddIntegerOption(1, FText::FromString(TEXT("中")));
			VisualEffectDataObject->AddIntegerOption(2, FText::FromString(TEXT("高")));
			VisualEffectDataObject->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			VisualEffectDataObject->AddIntegerOption(4, FText::FromString(TEXT("影视级")));
			VisualEffectDataObject->SetDefaultValueFromInteger(3); // 默认值为极高视觉效果
			
			// 同样，对于视觉效果，我们可以直接使用 Unreal Engine 内置的 UGameUserSettings 类来处理视觉效果的获取和设置。
			VisualEffectDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
			VisualEffectDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));
			VisualEffectDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改

			const FText VisualEffectDescription = GET_DESCRIPTION_TEXT("VisualEffectDescKey");
			VisualEffectDataObject->SetDataDescriptionRichText(VisualEffectDescription);
			
			VisualEffectDataObject->AddEditDependencyData(CreatedOverallQualityDataObject);
			CreatedOverallQualityDataObject->AddEditDependencyData(VisualEffectDataObject);
			GraphicsCategoryCollection->AddChildListData(VisualEffectDataObject);
			
		}

		// 反射质量
		{
			UListDataObject_StringInteger* ReflectionQualityDataObject = NewObject<UListDataObject_StringInteger>(GraphicsCategoryCollection, UListDataObject_StringInteger::StaticClass());
			ReflectionQualityDataObject->SetDataID(FName("ReflectionQuality"));
			ReflectionQualityDataObject->SetDataDisplayName(FText::FromString(TEXT("反射质量")));
			ReflectionQualityDataObject->AddIntegerOption(0, FText::FromString(TEXT("低")));
			ReflectionQualityDataObject->AddIntegerOption(1, FText::FromString(TEXT("中")));
			ReflectionQualityDataObject->AddIntegerOption(2, FText::FromString(TEXT("高")));
			ReflectionQualityDataObject->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			ReflectionQualityDataObject->AddIntegerOption(4, FText::FromString(TEXT("影视级")));
			ReflectionQualityDataObject->SetDefaultValueFromInteger(3); // 默认值为极高反射质量
			
			// 同样，对于反射质量，我们可以直接使用 Unreal Engine 内置的 UGameUserSettings 类来处理反射质量的获取和设置。
			ReflectionQualityDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
			ReflectionQualityDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));
			ReflectionQualityDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改

			const FText ReflectionQualityDescription = GET_DESCRIPTION_TEXT("ReflectionQualityDescKey");
			ReflectionQualityDataObject->SetDataDescriptionRichText(ReflectionQualityDescription);
			
			ReflectionQualityDataObject->AddEditDependencyData(CreatedOverallQualityDataObject);
			CreatedOverallQualityDataObject->AddEditDependencyData(ReflectionQualityDataObject);
			GraphicsCategoryCollection->AddChildListData(ReflectionQualityDataObject);
		}

		// 后处理质量
		{
			UListDataObject_StringInteger* PostProcessingQualityDataObject = NewObject<UListDataObject_StringInteger>(GraphicsCategoryCollection, UListDataObject_StringInteger::StaticClass());
			PostProcessingQualityDataObject->SetDataID(FName("PostProcessingQuality"));
			PostProcessingQualityDataObject->SetDataDisplayName(FText::FromString(TEXT("后处理质量")));
			PostProcessingQualityDataObject->AddIntegerOption(0, FText::FromString(TEXT("低")));
			PostProcessingQualityDataObject->AddIntegerOption(1, FText::FromString(TEXT("中")));
			PostProcessingQualityDataObject->AddIntegerOption(2, FText::FromString(TEXT("高")));
			PostProcessingQualityDataObject->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			PostProcessingQualityDataObject->AddIntegerOption(4, FText::FromString(TEXT("影视级")));
			PostProcessingQualityDataObject->SetDefaultValueFromInteger(3); // 默认值为极高后处理质量
			
			// 同样，对于后处理质量，我们可以直接使用 Unreal Engine 内置的 UGameUserSettings 类来处理后处理质量的获取和设置。
			PostProcessingQualityDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
			PostProcessingQualityDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));
			PostProcessingQualityDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改

			const FText PostProcessingQualityDescription = GET_DESCRIPTION_TEXT("PostProcessingQualityDescKey");
			PostProcessingQualityDataObject->SetDataDescriptionRichText(PostProcessingQualityDescription);
			
			PostProcessingQualityDataObject->AddEditDependencyData(CreatedOverallQualityDataObject);
			CreatedOverallQualityDataObject->AddEditDependencyData(PostProcessingQualityDataObject);
			GraphicsCategoryCollection->AddChildListData(PostProcessingQualityDataObject);
		}
	}

	// 帧率设置
	{
		UListDataObject_Collection* FrameRateCategoryCollection = NewObject<UListDataObject_Collection>(VideoCollectionDataObject);
		FrameRateCategoryCollection->SetDataID(FName("FrameRateCategoryCollection"));
		FrameRateCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("帧率设置")));
		VideoCollectionDataObject->AddChildListData(FrameRateCategoryCollection);

		// 垂直同步
		{
			UListDataObject_StringBool* VSyncDataObject = NewObject<UListDataObject_StringBool>(FrameRateCategoryCollection, UListDataObject_StringBool::StaticClass());
			VSyncDataObject->SetDataID(FName("VSync"));
			VSyncDataObject->SetDataDisplayName(FText::FromString(TEXT("垂直同步")));
			VSyncDataObject->OverrideDisplayTrueText(FText::FromString(TEXT("开启")));
			VSyncDataObject->OverrideDisplayFalseText(FText::FromString(TEXT("关闭")));
			VSyncDataObject->SetFalseAsDefaultValue();
			VSyncDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
			VSyncDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));
			VSyncDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改

			const FText VSyncDescription = GET_DESCRIPTION_TEXT("VSyncDescKey");
			VSyncDataObject->SetDataDescriptionRichText(VSyncDescription);

			// 垂直同步选项应该只在全屏模式下可用，因此我们添加一个依赖关系到全屏模式选项。
			FOptionDataEditConditionDescriptor FullScreenCondition;
			FullScreenCondition.SetEditConditionFunction(
				[CreatedWindowModeDataObject]()->bool
				{
					return CreatedWindowModeDataObject->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;
				}
			);
			FullScreenCondition.SetDisabledRichReason(TEXT("\n\n<Warning>仅在全屏模式下可用</>\n<Warning>垂直同步选项仅在全屏模式下可用</>"));
			FullScreenCondition.SetForcedStringValue(TEXT("false"));
			VSyncDataObject->AddEditCondition(FullScreenCondition);
			
			FrameRateCategoryCollection->AddChildListData(VSyncDataObject);
			
		}

		// 帧率限制
		{
			UListDataObject_String* FrameRateLimitDataObject = NewObject<UListDataObject_String>(FrameRateCategoryCollection, UListDataObject_String::StaticClass());
			FrameRateLimitDataObject->SetDataID(FName("FrameRateLimit"));
			FrameRateLimitDataObject->SetDataDisplayName(FText::FromString(TEXT("帧率限制")));
			FrameRateLimitDataObject->AddDynamicOptionsString(LexToString(30.f), FText::FromString(TEXT("30 FPS")));
			FrameRateLimitDataObject->AddDynamicOptionsString(LexToString(60.f), FText::FromString(TEXT("60 FPS")));
			FrameRateLimitDataObject->AddDynamicOptionsString(LexToString(90.f), FText::FromString(TEXT("90 FPS")));
			FrameRateLimitDataObject->AddDynamicOptionsString(LexToString(120.f), FText::FromString(TEXT("120 FPS")));
			FrameRateLimitDataObject->AddDynamicOptionsString(LexToString(0.f), FText::FromString(TEXT("无限制")));
			FrameRateLimitDataObject->SetDefaultValueFromString(LexToString(0.f)); // 默认值为无限制不锁帧

			const FText FrameRateLimitDescription = GET_DESCRIPTION_TEXT("FrameRateLimitDescKey");
			FrameRateLimitDataObject->SetDataDescriptionRichText(FrameRateLimitDescription);
			
			// 帧率限制选项可以直接使用 Unreal Engine 内置的 UGameUserSettings 类来处理帧率限制的获取和设置。
			FrameRateLimitDataObject->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFrameRateLimit));
			FrameRateLimitDataObject->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));
			FrameRateLimitDataObject->SetShouldApplyChangeImmediately(true); // 设置为立即应用更改
			FrameRateCategoryCollection->AddChildListData(FrameRateLimitDataObject);
		}
		
	}

	RegisteredOptionsTabCollections.Add(VideoCollectionDataObject);
}
