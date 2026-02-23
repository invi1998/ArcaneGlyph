// INVI_1998 All Rights Reserved.


#include "Scan/ScanEffectComponent.h"


UScanEffectComponent::UScanEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UScanEffectComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeSubsystem();

	if (bAutoStartOnBeginPlay)
	{
		StartScan();
	}
}

void UScanEffectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopScan();
	Super::EndPlay(EndPlayReason);
}

void UScanEffectComponent::InitializeSubsystem()
{
	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		ScanSubsystem = GameInstance->GetSubsystem<UScanEffectSubsystem>();
	}
}

void UScanEffectComponent::StartScan()
{
	if (ScanSubsystem)
	{
		ScanSubsystem->StartScanEffect(GetOwner(), DefaultConfig);
	}
}

void UScanEffectComponent::StartScanWithConfig(const FScanEffectConfig& Config)
{
	if (ScanSubsystem)
	{
		ScanSubsystem->StartScanEffect(GetOwner(), Config);
	}
}

void UScanEffectComponent::StartScanWithPreset(FName PresetName)
{
	if (ScanSubsystem)
	{
		ScanSubsystem->StartScanWithPreset(GetOwner(), PresetName);
	}
}

void UScanEffectComponent::StopScan()
{
	if (ScanSubsystem && GetOwner())
	{
		ScanSubsystem->StopScanEffect(GetOwner());
	}
}

bool UScanEffectComponent::IsScanning() const
{
	if (ScanSubsystem && GetOwner())
	{
		return ScanSubsystem->IsScanning(GetOwner());
	}
	return false;
}

