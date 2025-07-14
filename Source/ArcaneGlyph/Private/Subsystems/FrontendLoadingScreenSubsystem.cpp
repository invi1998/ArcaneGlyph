// INVI_1998 All Rights Reserved.


#include "Subsystems/FrontendLoadingScreenSubsystem.h"

#include "ArcaneDebugHelper.h"
#include "PreLoadScreenManager.h"
#include "Blueprint/UserWidget.h"
#include "FrontendSettings/FrontendLoadingScreenSettings.h"

bool UFrontendLoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		// 如果不是专用服务器实例，就查找是否有派生类，如果没有派生类，则不创建子系统
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	// 如果是专用服务器实例，则不创建子系统（因为服务器不需要UI）
	return false;
}

void UFrontendLoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	// 绑定地图预加载事件
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::OnMapPreLoaded);

	// 绑定地图加载完成事件
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnMapPostLoaded);
}

void UFrontendLoadingScreenSubsystem::Deinitialize()
{
	// 解绑地图预加载事件
	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);

	// 解绑地图加载完成事件
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	
	Super::Deinitialize();
}

UWorld* UFrontendLoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	if (UGameInstance* OwningGameInstance = GetGameInstance())
	{
		return OwningGameInstance->GetWorld();
	}

	return nullptr;
}

void UFrontendLoadingScreenSubsystem::Tick(float DeltaTime)
{
	TryUpdateLoadingScreen();
}

ETickableTickType UFrontendLoadingScreenSubsystem::GetTickableTickType() const
{
	if (IsTemplate())
	{
		// 如果是模板，则不需要Tick，因为模板不会被实例化
		return ETickableTickType::Never;
	}

	// 如果不是模板，则返回条件Tick类型，即按照条件进行Tick
	return ETickableTickType::Conditional;
}

bool UFrontendLoadingScreenSubsystem::IsTickable() const
{
	// 检查游戏实例和游戏视口客户端是否存在
	return GetGameInstance() && GetGameInstance()->GetGameViewportClient();
}

TStatId UFrontendLoadingScreenSubsystem::GetStatId() const
{
	// 返回一个唯一的统计ID，用于跟踪Tickable对象的性能
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFrontendLoadingScreenSubsystem, STATGROUP_Tickables);
}

void UFrontendLoadingScreenSubsystem::OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName)
{
	if (WorldContext.OwningGameInstance != GetGameInstance())
	{
		return;
	}

	SetTickableTickType(ETickableTickType::Conditional);
	bIsCurrentlyLoadingMap = true;
	TryUpdateLoadingScreen();
}

void UFrontendLoadingScreenSubsystem::OnMapPostLoaded(UWorld* LoadedWorld)
{
	if (LoadedWorld && LoadedWorld->GetGameInstance() == GetGameInstance())
	{
		bIsCurrentlyLoadingMap = false;
	}
}

// 预加载画面是否激活
bool UFrontendLoadingScreenSubsystem::IsPreLoadingScreenActive() const
{
	FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get();
	if (PreLoadScreenManager)
	{
		// 检查预加载屏幕管理器是否存在，并且当前是否有有效的活动预加载屏幕
		return PreLoadScreenManager->HasValidActivePreLoadScreen();
	}

	return false;
}

bool UFrontendLoadingScreenSubsystem::ShouldShowLoadingScreen()
{
	if (const UFrontendLoadingScreenSettings* LoadingScreenSettings = GetDefault<UFrontendLoadingScreenSettings>())
	{
		// 检查是否启用了加载界面
		if (GIsEditor && !LoadingScreenSettings->bShouldLoadingScreenInEditor)
		{
			// 如果在编辑器中且不启用加载界面，则不显示加载界面
			return false;
		}

		// 检查世界对象是否需要加载界面
		if (CheckTheNeedToShowLoadingScreen())
		{
			// 如果仍需显示加载画面，就无需将世界场景渲染至视口
			GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;
			return true;
		}
		
		CurrentLoadingReason = TEXT("当前加载区域等待纹理流送");

		// 无需显示加载界面，将世界场景渲染至视口
		GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = false;

		const float CurrentTime = FPlatformTime::Seconds();

		if (HoldLoadingScreenStartupTime < 0.f)
		{
			// 如果没有设置启动加载界面的持续时间，则不显示加载界面
			HoldLoadingScreenStartupTime = CurrentTime;
		}

		// 计算自启动加载界面以来的经过时间
		const float ElapsedTime = CurrentTime - HoldLoadingScreenStartupTime;

		// 在第一次 tick 时，我们的当前时间将等于这个加载界面的起始时间
		// 但从第二个 tick 开始，当前时间应大于整个加载屏幕的起始时间
		// 计算已用时间的方法 是通过当前时间减去加载界面的起始时间来实现的

		// 如果经过的时间小于加载界面的持续时间，则显示加载界面（即我们希望在完成地图加载后继续持续一段时间显示加载界面）
		// 加载屏幕设置中的这个变量用于消除模糊纹理
		if (ElapsedTime < LoadingScreenSettings->HoldLoadingScreenExtraSeconds)
		{
			// 如果经过的时间大于或等于加载界面的启动时间，则显示加载界面
			return true;
		}
		
	}

	return false;
}

bool UFrontendLoadingScreenSubsystem::CheckTheNeedToShowLoadingScreen()
{
	if (bIsCurrentlyLoadingMap)
	{
		// 如果当前正在加载地图，则需要显示加载界面
		CurrentLoadingReason = TEXT("关卡加载中");
		return true;
	}
	UWorld* CurrentWorld = GetGameInstance()->GetWorld();
	if (!CurrentWorld)
	{
		CurrentLoadingReason = TEXT("世界初始化中");
		return true;
	}

	if (!CurrentWorld->HasBegunPlay())
	{
		CurrentLoadingReason = TEXT("即将开始游戏");
		return true;
	}

	if (!CurrentWorld->GetFirstPlayerController())
	{
		CurrentLoadingReason = TEXT("玩家控制器未就绪");
		return true;
	}

	// 可以检查游戏状态、玩家状态或玩家角色 actor 组件
	return false;
}

void UFrontendLoadingScreenSubsystem::TryDisplayLoadingScreen()
{
	// 首先检查是否已经创建了加载界面控件
	if (CachedCreatedLoadingScreenWidget) return;

	if (const UFrontendLoadingScreenSettings* LoadingScreenSettings = GetDefault<UFrontendLoadingScreenSettings>())
	{
		if (TSubclassOf<UUserWidget> LoadingScreenClass = LoadingScreenSettings->GetLoadingScreenWidgetClassChecked())
		{
			// 获取到这个控件后，我们就可以尝试将其绘制到视口中
			// 因为我们正在关卡过度期间绘制控件
			// 所以我们不能再依赖世界场景或玩家控制器了
			// 我们需要依赖游戏实例来实现
			UUserWidget* CreatedWidget = UUserWidget::CreateWidgetInstance(
				*GetGameInstance(),
				LoadingScreenClass,
				NAME_None
			);
			if (CreatedWidget)
			{
				CachedCreatedLoadingScreenWidget = CreatedWidget->TakeWidget();
				// 将加载界面添加到游戏视口
				GetGameInstance()->GetGameViewportClient()->AddViewportWidgetContent(
					CachedCreatedLoadingScreenWidget.ToSharedRef(),
					1000		// 数值较大的控件会显示在数值较小的控件上方
				);
			}
		}
	}
}

void UFrontendLoadingScreenSubsystem::TryUpdateLoadingScreen()
{
	// 首先需要检查当前是否存在启动加载界面
	if (IsPreLoadingScreenActive()) return;

	// 检查是否应该显示加载界面
	if (ShouldShowLoadingScreen())
	{
		TryDisplayLoadingScreen();
		// 显示加载界面
		OnLoadingReasonChanged.Broadcast(CurrentLoadingReason);
	}
	else
	{
		// 移除当前加载界面
		// 通知加载完成
		// 禁用Tick
		SetTickableTickType(ETickableTickType::Never);
	}

	
}
