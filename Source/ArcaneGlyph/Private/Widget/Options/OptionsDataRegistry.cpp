// INVI_1998 All Rights Reserved.


#include "Widget/Options/OptionsDataRegistry.h"

#include "Widget/Options/DataObject/ListDataObject_Collection.h"

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitControlsCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayCollectionDataObject = NewObject<UListDataObject_Collection>(this, UListDataObject_Collection::StaticClass());
	GameplayCollectionDataObject->SetDataID(FName("GameplayTabCollection"));
	GameplayCollectionDataObject->SetDataDisplayName(FText::FromString(TEXT("游戏")));

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
