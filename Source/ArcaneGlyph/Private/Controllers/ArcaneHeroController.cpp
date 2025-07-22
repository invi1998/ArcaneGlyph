// INVI_1998 All Rights Reserved.


#include "ArcaneGlyph/Public/Controllers/ArcaneHeroController.h"

#include "CommonInputSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraActor.h"
#include "Characters/ArcaneHeroCharacter.h"
#include "Component/Combat/HeroCombatComponent.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Kismet/GameplayStatics.h"


AArcaneHeroController::AArcaneHeroController()
{
	// IGenericTeamAgentInterface::SetGenericTeamId(0);
	// 这里和AIController中的设置团队 ID 的方式不同，AIController中是通过 AAIController::SetGenericTeamId 来设置团队 ID
	// 而因为我们这里没有继承自 AAIController，而 IGenericTeamAgentInterface::SetGenericTeamId 设置团队 ID 是一个空函数，所以这里设置团队 ID 是无效的
	// 所以我们在玩家控制器里新增一个成员变量来保存团队 ID FGenericTeamId HeroTeamID;	// 英雄团队 ID（玩家团队 ID）
	HeroTeamID = FGenericTeamId(0);
}

FGenericTeamId AArcaneHeroController::GetGenericTeamId() const
{
	return HeroTeamID;
}

void AArcaneHeroController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName("Default"), FoundCameras);

	if (!FoundCameras.IsEmpty())
	{
		// 如果找到了摄像机，就将玩家控制器的视角设置为摄像机
		SetViewTarget(FoundCameras[0]);
	}
	else
	{
		// 如果没有找到摄像机，就使用默认的视角
		SetViewTarget(InPawn);
	}

	UFrontendGameUserSettings* GameUserSettings = UFrontendGameUserSettings::Get();
	if (GameUserSettings && (
		GameUserSettings->GetLastCPUBenchmarkResult() == -1.f || // 如果没有进行过 CPU 性能测试
		GameUserSettings->GetLastGPUBenchmarkResult() == -1.f	// 如果没有进行过 GPU 性能测试
		)) 
	{
		// 开始性能测试等
		GameUserSettings->RunHardwareBenchmark();	// 运行硬件性能测试
		GameUserSettings->ApplyHardwareBenchmarkResults();	// 应用硬件性能测试结果
	}

}

void AArcaneHeroController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

		// 监听设备切换事件
		if (UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
		{
			CommonInputSubsystem->OnInputMethodChangedNative.AddUObject(this, &ThisClass::HandleInputDeviceChanged);
		}
	}
}

void AArcaneHeroController::HandleInputDeviceChanged(ECommonInputType NewInputType)
{
	if (!InputSubsystem) return;

	if (AArcaneHeroCharacter* Hero = Cast<AArcaneHeroCharacter>(GetPawn()))
	{
		// 处理输入设备切换事件
		if (UHeroCombatComponent* HeroCombatComponent = Hero->GetHeroCombatComponent())
		{
			HeroCombatComponent->ReloadWeaponMappingContext(InputSubsystem);
		}
	}

}



