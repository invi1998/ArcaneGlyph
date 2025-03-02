﻿// INVI_1998 All Rights Reserved.


#include "AbilitySystem/Abilities/HeroGameplayAbility_TargetLock.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneDebugHelper.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/ArcaneHeroCharacter.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widget/ArcaneWidgetBase.h"
#include "Controllers/ArcaneHeroController.h"
#include "ArcaneGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                      const FGameplayEventData* TriggerEventData)
{
	InitTargetLockMovement();
	
	TryLockTargetLock();

	InitTargetLockInputMappingContext();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	ResetTargetLockMovement();
	Cleanup();
	ResetTargetLockInputMappingContext();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::OnTargetLockTick(float DeltaTime)
{
	if (!CurrentLockedActor ||
		UArcaneBlueprintFunctionLibrary::NativeDoesActorHasGameplayTag(CurrentLockedActor, ArcaneGameplayTags::Shared_Status_Dead) ||
		UArcaneBlueprintFunctionLibrary::NativeDoesActorHasGameplayTag(GetHeroCharacterFromActorInfo(), ArcaneGameplayTags::Shared_Status_Dead))
	{
		CancelTargetLockAbility();
		return;
	}

	SetTargetLockWidgetPosition();

	// 如果当前角色正在翻滚或者正在格挡，就不要旋转角色
	const bool bShouldOverrideRotation = !UArcaneBlueprintFunctionLibrary::NativeDoesActorHasGameplayTag(GetHeroCharacterFromActorInfo(), ArcaneGameplayTags::Player_Status_Rolling)
										&& !UArcaneBlueprintFunctionLibrary::NativeDoesActorHasGameplayTag(GetHeroCharacterFromActorInfo(), ArcaneGameplayTags::Player_Status_Blocking);

	if (bShouldOverrideRotation)
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetHeroCharacterFromActorInfo()->GetActorLocation(), CurrentLockedActor->GetActorLocation());
		// 差值旋转，使角色平滑旋转
		// 同时，在目标锁定期间，角色的摄像机也应该朝向目标
		LookAtRotation -= FRotator(TargetLockCameraOffsetDistance, 0.f, 0.f);
		
		const FRotator CurrentControllerRotation = GetHeroControllerFromActorInfo()->GetControlRotation();
		const FRotator TargetRot = FMath::RInterpTo(CurrentControllerRotation, LookAtRotation, DeltaTime, TargetLockRotationInterpSpeed);

		GetHeroControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
		// 设置了控制器的旋转，角色的旋转也会跟着改变
		GetHeroCharacterFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
	}
}

void UHeroGameplayAbility_TargetLock::SwitchLockTarget(const FGameplayTag& InSwitchDirectionTag)
{
	// 先刷新一次场景里的可锁定角色信息
	GetAvailableTargetToLock();

	TArray<AActor*> AvailableLeftActors{};
	TArray<AActor*> AvailableRightActors{};
	AActor* NewLockTarget = nullptr;

	GetAvailableActorsAroundTarget(AvailableLeftActors, AvailableRightActors);

	if (InSwitchDirectionTag.MatchesTagExact(ArcaneGameplayTags::Player_Event_SwitchLockTarget_Left))
	{
		// 向左切换锁定
		NewLockTarget = GetNearestTargetFromAvailable(AvailableLeftActors);
	}
	else if (InSwitchDirectionTag.MatchesTagExact(ArcaneGameplayTags::Player_Event_SwitchLockTarget_Right))
	{
		// 向右切换锁定
		NewLockTarget = GetNearestTargetFromAvailable(AvailableRightActors);
	}

	if (NewLockTarget != nullptr)
	{
		CurrentLockedActor = NewLockTarget;
	}
	
}

void UHeroGameplayAbility_TargetLock::TryLockTargetLock()
{
	GetAvailableTargetToLock();

	if (AvailableTargetToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	CurrentLockedActor = GetNearestTargetFromAvailable(AvailableTargetToLock);
	if (CurrentLockedActor)
	{
		DrawTargetLockWidget();

		SetTargetLockWidgetPosition();
	}
	else
	{
		CancelTargetLockAbility();
	}
	
}

void UHeroGameplayAbility_TargetLock::GetAvailableTargetToLock()
{
	AvailableTargetToLock.Empty();
	TArray<FHitResult> BoxTraceHitResults;
	
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetHeroCharacterFromActorInfo(),		// 从ActorInfo获取英雄角色
		GetHeroCharacterFromActorInfo()->GetActorLocation(),		// 从英雄角色获取位置
		GetHeroCharacterFromActorInfo()->GetActorLocation() + GetHeroCharacterFromActorInfo()->GetActorForwardVector() * LockDistance,		// 从英雄角色获取前方1000米的位置
		TraceBoxSize * 0.5f,		// 射线盒大小
		GetHeroCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),		// 从英雄角色获取前方向量
		TargetLockObjectTypes,		// 指定检测目标的对象类型
		false,		// 是否检测复杂碰撞
		{},		// 忽略的Actor
		bShowDebugTrace ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,		// 是否显示调试射线
		BoxTraceHitResults,		// 射线命中结果
		true		// 是否忽略自身
		);

	// 遍历射线命中结果
	for (const FHitResult& HitResult : BoxTraceHitResults)
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (HitActor != GetHeroCharacterFromActorInfo())
			{
				AvailableTargetToLock.AddUnique(HitActor);

			}
		}
	}
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestTargetFromAvailable(const TArray<AActor*> InAvailableActors)
{
	float ClosestDistance = TNumericLimits<float>::Max();

	// 使用GameplayStatics的FindNearestActor函数来查找最近的Actor，该函数会返回最近的Actor和距离
	return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, ClosestDistance);
	
}

void UHeroGameplayAbility_TargetLock::DrawTargetLockWidget()
{
	if (!IsValid(TargetLockWidget))
	{
		checkf(TargetLockWidgetClass, TEXT("TargetLockWidgetClass is not set in %s!"), *GetName());

		TargetLockWidget = CreateWidget<UArcaneWidgetBase>(GetHeroControllerFromActorInfo(), TargetLockWidgetClass);

		check(TargetLockWidget);

	}
	
	TargetLockWidget->AddToViewport();
}

void UHeroGameplayAbility_TargetLock::SetTargetLockWidgetPosition()
{
	if (IsValid(TargetLockWidget) && CurrentLockedActor)
	{
		FVector2D ScreenPosition;

		// 将世界位置投影到屏幕位置
		UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
			GetHeroControllerFromActorInfo(),
			CurrentLockedActor->GetActorLocation(),
			ScreenPosition,
			true		// 是否考虑DPIScale
		);

		if (TargetLockWidgetSize.IsZero())
		{
			TargetLockWidget->WidgetTree->ForEachWidget([this](UWidget* Widget)
			{
				if (USizeBox* SizeBox = Cast<USizeBox>(Widget))
				{
					TargetLockWidgetSize.X = SizeBox->GetWidthOverride();
					TargetLockWidgetSize.Y = SizeBox->GetHeightOverride();
				}
			});
		}

		ScreenPosition -= TargetLockWidgetSize * 0.5f;
		
		// 设置小部件的屏幕位置，第二个参数为false表示移除DPIScale，因为我们已经在之前计算投影位置时考虑了DPIScale
		TargetLockWidget->SetPositionInViewport(ScreenPosition, false);
	}
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMovement()
{
	CachedDefaultMaxWalkSpeed = GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed;
	GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = TargetLockMaxWalkSpeed;
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMovement()
{
	if (CachedDefaultMaxWalkSpeed > 0.f)
    {
        GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
    }
}

void UHeroGameplayAbility_TargetLock::InitTargetLockInputMappingContext()
{
	const ULocalPlayer* OwningLocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwningLocalPlayer);
	check(Subsystem);

	Subsystem->AddMappingContext(TargetLockInputMappingContext, 3);
	
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockInputMappingContext()
{
	if (!GetHeroControllerFromActorInfo()) return;
	
	const ULocalPlayer* OwningLocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwningLocalPlayer);
	check(Subsystem);

	Subsystem->RemoveMappingContext(TargetLockInputMappingContext);
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorOnLeft, TArray<AActor*>& OutActorOnRight)
{
	if (!IsValid(CurrentLockedActor) || AvailableTargetToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	const FVector PlayerLocation = GetHeroCharacterFromActorInfo()->GetActorLocation();
	const FVector CurrentLockedTargetLocation = CurrentLockedActor->GetActorLocation();
	const FVector PlayerToCurrentNormalized = (CurrentLockedTargetLocation - PlayerLocation).GetSafeNormal();

	// 然后遍历AvailableTargetToLock，将他们和玩家所在位置的连线 和 玩家到当前锁定目标的位置的连线 做叉乘（得到角度信息，根据角度信息判段左右）
	for (AActor* ActorToCheck : AvailableTargetToLock)
	{
		if (!ActorToCheck || ActorToCheck == CurrentLockedActor) continue;

		const FVector PlayerToCheckNormalized = (ActorToCheck->GetActorLocation() - PlayerLocation).GetSafeNormal();

		// 获取叉积
		const FVector CrossVector = FVector::CrossProduct(PlayerToCurrentNormalized, PlayerToCheckNormalized);

		if (CrossVector.Z > 0.f)
		{
			OutActorOnRight.AddUnique(ActorToCheck);
		}
		else
		{
			OutActorOnLeft.AddUnique(ActorToCheck);
		}
		
	}

	
}

void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroGameplayAbility_TargetLock::Cleanup()
{
	AvailableTargetToLock.Empty();
	CurrentLockedActor = nullptr;

	if (IsValid(TargetLockWidget))
	{
		TargetLockWidget->RemoveFromParent();
		TargetLockWidget = nullptr;
	}

	TargetLockWidgetSize = FVector2D::ZeroVector;
	
}
