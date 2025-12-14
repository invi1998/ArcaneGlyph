// INVI_1998 All Rights Reserved.


#include "ArcaneBlueprintFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArcaneDebugHelper.h"
#include "ArcaneGameplayTags.h"
#include "CommonInputSubsystem.h"
#include "EngineUtils.h"
#include "GenericTeamAgentInterface.h"
#include "KismetAnimationLibrary.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "AbilitySystem/ArcaneAttributeSet.h"
#include "AnimInstances/ArcaneCharacterAnimInstance.h"
#include "ArcaneTypes/ArcaneCountDownAction.h"
#include "Characters/ArcaneHeroCharacter.h"
#include "Component/Combat/PawnCombatComponent.h"
#include "Component/UI/PawnUIComponent.h"
#include "FrontendSettings/FrontendDeveloperSettings.h"
#include "Game/ArcaneGameInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SaveGame/ArcaneSaveGame.h"


UArcaneAbilitySystemComponent* UArcaneBlueprintFunctionLibrary::NativeGetArcaneASCFromActor(AActor* InActor)
{
	// 只要InActor实现了 IAbilitySystemInterface 接口的 GetAbilitySystemComponent函数，就可以获取到AbilitySystemComponent
	return Cast<UArcaneAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

UArcaneAbilitySystemComponent* UArcaneBlueprintFunctionLibrary::GetArcaneASCFromActor(AActor* InActor)
{
	return NativeGetArcaneASCFromActor(InActor);
}

void UArcaneBlueprintFunctionLibrary::AddGameplayTagToActorIfNotHas(AActor* InActor, FGameplayTag InTag)
{
	if (UArcaneAbilitySystemComponent* ArcaneASC = NativeGetArcaneASCFromActor(InActor))
	{
		if (!ArcaneASC->HasMatchingGameplayTag(InTag))
		{
			ArcaneASC->AddLooseGameplayTag(InTag);
		}
	}
}

void UArcaneBlueprintFunctionLibrary::RemoveGameplayTagFromActorIfHas(AActor* InActor, FGameplayTag InTag)
{
	if (UArcaneAbilitySystemComponent* ArcaneASC = NativeGetArcaneASCFromActor(InActor))
	{
		if (ArcaneASC->HasMatchingGameplayTag(InTag))
		{
			ArcaneASC->RemoveLooseGameplayTag(InTag);
		}
	}
}

bool UArcaneBlueprintFunctionLibrary::NativeDoesActorHasGameplayTag(AActor* InActor, FGameplayTag InTag)
{
	if (UArcaneAbilitySystemComponent* ArcaneASC = NativeGetArcaneASCFromActor(InActor))
	{
		return ArcaneASC->HasMatchingGameplayTag(InTag);
	}
	return false;
}

void UArcaneBlueprintFunctionLibrary::BP_DoesActorHasGameplayTag(AActor* InActor, FGameplayTag InTag, EArcaneConfirmType& ConfirmType)
{
	// 通过调用原生函数来判断Actor是否拥有指定的GameplayTag,并将结果赋值给ConfirmType
	ConfirmType = NativeDoesActorHasGameplayTag(InActor, InTag) ? EArcaneConfirmType::YES : EArcaneConfirmType::NO;
}

bool UArcaneBlueprintFunctionLibrary::DoseActorHasGameplayTag(AActor* InActor, FGameplayTag InTag)
{
	return NativeDoesActorHasGameplayTag(InActor, InTag);
}

UPawnCombatComponent* UArcaneBlueprintFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);

	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}
	return nullptr;
}

UPawnCombatComponent* UArcaneBlueprintFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EArcaneValidType& ValidType)
{
	// 通过调用原生函数来获取Actor的PawnCombatComponent，并将结果赋值给ValidType
	UPawnCombatComponent* PawnCombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
	ValidType = PawnCombatComponent ? EArcaneValidType::Valid : EArcaneValidType::InValid;
	return PawnCombatComponent;
}

UPawnUIComponent* UArcaneBlueprintFunctionLibrary::NativeGetPawnUIComponentFromActor(AActor* InActor)
{
	check(InActor);

	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(InActor))
	{
		return PawnUIInterface->GetPawnUIComponent();
	}
	return nullptr;
}

bool UArcaneBlueprintFunctionLibrary::IsTargetPawnHostile(APawn* InQueryPawn, APawn* InTargetPawn)
{
	check(InQueryPawn && InTargetPawn);

	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(InQueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(InTargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}

	return false;
}

void UArcaneBlueprintFunctionLibrary::BroadcastGameplayTagChangedToUIComponent(AActor* InActor, FGameplayTag InTag, bool bAddTag)
{
	check(InActor);

	if (UPawnUIComponent* PawnUIComponent = NativeGetPawnUIComponentFromActor(InActor))
	{
		if (InTag.MatchesTagExact(ArcaneGameplayTags::Enemy_Ability_Melee) || InTag.MatchesTagExact(ArcaneGameplayTags::Enemy_Ability_Range))
		{
			PawnUIComponent->OnHaveAttackGameplayTag.Broadcast(InTag);
		}
	}
}

float UArcaneBlueprintFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, int32 InLevel)
{
	return InScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UArcaneBlueprintFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAndleDifference)
{
	check(InAttacker && InVictim);

	// 获取受害者的前向向量
	const FVector VictimForward = InVictim->GetActorForwardVector();
	// 获取攻击者到受害者的单位向量
	const FVector AttackerToVictim = (InVictim->GetActorLocation() - InAttacker->GetActorLocation()).GetSafeNormal();

	// 计算两个向量的点积（cos值）
	const float DotResult = FVector::DotProduct(VictimForward, AttackerToVictim);
	// 计算两个向量的夹角 cos值的反余弦值
	// const float Angle = FMath::Acos(DotResult) * (180.f / PI);
	OutAndleDifference = UKismetMathLibrary::DegAcos(DotResult);

	const FVector CrossResult = FVector::CrossProduct(VictimForward, AttackerToVictim);

	// 如果叉积的Z值大于0，则说明攻击者在受害者的左侧，否则在右侧
	if (CrossResult.Z < 0.f)
	{
		OutAndleDifference *= -1.f;
	}
	
	if (OutAndleDifference >= -45.f && OutAndleDifference <= 45.f)
	{
		return ArcaneGameplayTags::Shared_Status_HitReact_Front;
	}
	else if (OutAndleDifference > 45.f && OutAndleDifference <= 135.f)
	{
		return ArcaneGameplayTags::Shared_Status_HitReact_Right;
	}
	else if (OutAndleDifference < -45.f && OutAndleDifference >= -135.f)
	{
		return ArcaneGameplayTags::Shared_Status_HitReact_Left;
	}
	else
	{
		return ArcaneGameplayTags::Shared_Status_HitReact_Back;
	}

}

bool UArcaneBlueprintFunctionLibrary::IsCharacterEquippedWeapon(AActor* InActor)
{
	check(InActor);
	if (UPawnCombatComponent* PawnCombatComponent = NativeGetPawnCombatComponentFromActor(InActor))
	{
		return PawnCombatComponent->CurrentEquippedRightHandWeaponTag.IsValid() || PawnCombatComponent->CurrentEquippedLeftHandWeaponTag.IsValid();
	}
	return false;
}

void UArcaneBlueprintFunctionLibrary::SetCharacterMaxWalkSpeed(AActor* InActor, float InMaxWalkSpeed)
{
	check(InActor);
	if (ACharacter* Character = Cast<ACharacter>(InActor))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = InMaxWalkSpeed;
	}
}

float UArcaneBlueprintFunctionLibrary::GetCharacterMaxWalkSpeed(AActor* InActor)
{
	check(InActor);
	if (ACharacter* Character = Cast<ACharacter>(InActor))
	{
		return Character->GetCharacterMovement()->MaxWalkSpeed;
	}
	return 0.f;
}

bool UArcaneBlueprintFunctionLibrary::IsCurrentBlockValid(AActor* InAttacker, AActor* InDefender)
{
	check(InAttacker && InDefender);

	FVector AttackerForward = InAttacker->GetActorForwardVector();
	FVector DefenderForward = InDefender->GetActorForwardVector();
	// 获取攻击者和防御者的前向向量的点积（cos值）
	const float DotResult = FVector::DotProduct(AttackerForward, DefenderForward);

	// 如果cos值大于0.，则说明攻击者和在防御者前方，此时防御者的格挡是有效的
	return DotResult < -0.1f;
	
}

void UArcaneBlueprintFunctionLibrary::SetPlayerIsLeftHandUsingWeapon(AActor* InActor, bool bIsLeftHandUsingWeapon)
{
	check(InActor);
	if (AArcaneHeroCharacter* InHeroCharacter = Cast<AArcaneHeroCharacter>(InActor))
	{
		InHeroCharacter->bIsLeftHandUsingWeapon = bIsLeftHandUsingWeapon;
	}
}

bool UArcaneBlueprintFunctionLibrary::ApplyGameplayEffectSpecHandleToTarget(AActor* InInstigator, AActor* InTarget, const FGameplayEffectSpecHandle& InEffectSpecHandle)
{
	UArcaneAbilitySystemComponent* InstigatorASC = NativeGetArcaneASCFromActor(InInstigator);
	UArcaneAbilitySystemComponent* TargetASC = NativeGetArcaneASCFromActor(InTarget);

	if (InstigatorASC && TargetASC)
	{
		FActiveGameplayEffectHandle ActiveGameplayEffectHandle = InstigatorASC->ApplyGameplayEffectSpecToTarget(*InEffectSpecHandle.Data.Get(), TargetASC);

		return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
	}

	return false;
}

EArcaneMoveDirection UArcaneBlueprintFunctionLibrary::GetMoveDirection(
    EArcaneMovementDirectionMethod Method, const FVector& InputVector, float DeadZone)
{
    const FVector SafeInput = InputVector.GetSafeNormal2D();
    if (SafeInput.IsNearlyZero(DeadZone)) return EArcaneMoveDirection::None;

    switch (Method)
    {
    case EArcaneMovementDirectionMethod::AxisDominant:
        // Y轴决定前后，但Pawn的Forward方向可能与Y轴负方向一致
        return (FMath::Abs(SafeInput.Y) > FMath::Abs(SafeInput.X)) ?
            ((SafeInput.Y < 0) ? EArcaneMoveDirection::Forward : EArcaneMoveDirection::Backward) : // Y负为前，Y正为后
            ((SafeInput.X > 0) ? EArcaneMoveDirection::Right : EArcaneMoveDirection::Left);

    case EArcaneMovementDirectionMethod::AngleThreshold4:
    {
        // 将Y轴方向反转，确保角度计算与Pawn的Forward方向一致（Y负为前）
        const float Angle = FMath::RadiansToDegrees(FMath::Atan2(-SafeInput.Y, SafeInput.X));
        const float WrappedAngle = (Angle < 0) ? Angle + 360 : Angle;

        if (WrappedAngle >= 225 && WrappedAngle < 315)    return EArcaneMoveDirection::Forward;   // 270度 → Forward（Y负）
        if (WrappedAngle >= 315 || WrappedAngle < 45)    return EArcaneMoveDirection::Right;     // 0度 → Right
        if (WrappedAngle >= 45 && WrappedAngle < 135)    return EArcaneMoveDirection::Backward;  // 90度 → Backward（Y正）
        if (WrappedAngle >= 135 && WrappedAngle < 225)   return EArcaneMoveDirection::Left;      // 180度 → Left
        return EArcaneMoveDirection::None;
    }

    case EArcaneMovementDirectionMethod::AngleThreshold8:
    {
        constexpr float SectorSize = 45.0f;
        // 将Y轴方向反转，确保角度计算与Pawn的Forward方向一致（Y负为前）
        const float Angle = FMath::RadiansToDegrees(FMath::Atan2(-SafeInput.Y, SafeInput.X));
        const float NormalizedAngle = FMath::Fmod(Angle + 360 + 22.5f, 360);

        const int32 SectorIndex = FMath::FloorToInt(NormalizedAngle / SectorSize);
        switch (SectorIndex)
        {
        case 0:  return EArcaneMoveDirection::Right;         // 0度 → Right
        case 1:  return EArcaneMoveDirection::ForwardRight;  // 45度 → ForwardRight
        case 2:  return EArcaneMoveDirection::Forward;       // 90度 → Forward（Y负）
        case 3:  return EArcaneMoveDirection::ForwardLeft;   // 135度 → ForwardLeft
        case 4:  return EArcaneMoveDirection::Left;          // 180度 → Left
        case 5:  return EArcaneMoveDirection::BackwardLeft;  // 225度 → BackwardLeft
        case 6:  return EArcaneMoveDirection::Backward;      // 270度 → Backward（Y正）
        case 7:  return EArcaneMoveDirection::BackwardRight; // 315度 → BackwardRight
        default: return EArcaneMoveDirection::None;
        }
    }
    default:
        return EArcaneMoveDirection::None;
    }
}

void UArcaneBlueprintFunctionLibrary::CountDown(const UObject* WorldContextObject, float TotalTime,
	float UpdateInterval, float& OutRemainingTime, EArcaneCountDownActionInput CountDownActionInput,
	UPARAM(DisplayName="Output") EArcaneCountDownActionOutput& CountDownActionOutput, FLatentActionInfo LatentInfo)
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);

	if (!World) return;

	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
	FArcaneCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FArcaneCountDownAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);

	if (CountDownActionInput == EArcaneCountDownActionInput::Start)
	{
		// 如果找不到已存在的Action，就创建一个新的Action
		if (!FoundAction)
		{
			FArcaneCountDownAction* NewAction = new FArcaneCountDownAction(TotalTime, UpdateInterval, OutRemainingTime, CountDownActionOutput, LatentInfo);

			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, NewAction);
		}
	}

	if (CountDownActionInput == EArcaneCountDownActionInput::Cancel)
	{
		if (FoundAction)
		{
			FoundAction->CancelAction();
		}
	}

}

void UArcaneBlueprintFunctionLibrary::ToggleCharacterGravity(AActor* InActor, bool bEnableGravity)
{
	check(InActor);

	if (AArcaneCharacterBase* ArcaneCharacter = Cast<AArcaneCharacterBase>(InActor))
	{
		ArcaneCharacter->GetCharacterMovement()->SetMovementMode(bEnableGravity ? MOVE_Walking : MOVE_Flying);
	}
}

AActor* UArcaneBlueprintFunctionLibrary::GetPlayerCurrentLockedTarget(AActor* InActor)
{
	check(InActor);
	if (AArcaneHeroCharacter* ArcaneHeroCharacter = Cast<AArcaneHeroCharacter>(InActor))
	{
		return ArcaneHeroCharacter->CurrentLockedTargetActor;
	}
	return nullptr;
}

void UArcaneBlueprintFunctionLibrary::ToggleCharacterHegemony(AActor* InActor, bool bEnable)
{
	check(InActor);
	// 通过添加或者移除Tag来实现霸体状态的切换
	if (AArcaneCharacterBase* ArcaneCharacter = Cast<AArcaneCharacterBase>(InActor))
	{
		if (bEnable)
		{
			AddGameplayTagToActorIfNotHas(ArcaneCharacter, ArcaneGameplayTags::Shared_Status_Hegemony);
		}
		else
		{
			RemoveGameplayTagFromActorIfHas(ArcaneCharacter, ArcaneGameplayTags::Shared_Status_Hegemony);
		}
	}
}

int32 UArcaneBlueprintFunctionLibrary::GetCharacterCurrentSpark(AActor* InActor)
{
	check(InActor);

	if (AArcaneCharacterBase* ArcaneCharacter = Cast<AArcaneCharacterBase>(InActor))
	{
		int32 Spark = ArcaneCharacter->GetArcaneAttributeSet()->GetCurrentSpark();
		return Spark;
	}
	return 0;
}

float UArcaneBlueprintFunctionLibrary::GetCharacterCurrentEnergy(AActor* InActor)
{
	check(InActor);

	if (AArcaneCharacterBase* ArcaneCharacter = Cast<AArcaneCharacterBase>(InActor))
	{
		return ArcaneCharacter->GetArcaneAttributeSet()->GetCurrentEnergy();
	}
	return 0.f;
}

EArcaneMoveDirection UArcaneBlueprintFunctionLibrary::GetAnimLayerLocomotionDirection(AActor* InActor)
{
	// 获取角色的AnimInstance
	if (AArcaneCharacterBase* ArcaneCharacter = Cast<AArcaneCharacterBase>(InActor))
	{
		if (UArcaneCharacterAnimInstance* AnimInstance = Cast<UArcaneCharacterAnimInstance>(ArcaneCharacter->GetMesh()->GetAnimInstance()))
		{
			// 获取指定层的Locomotion方向
			float Dir = AnimInstance->GetLocomotionDirection();
			if ((Dir >= -180 && Dir < -135) || (Dir >= 135 && Dir < 180))
			{
				return EArcaneMoveDirection::Backward;
			}
			else if (Dir >= -135 && Dir < -45)
			{
				return EArcaneMoveDirection::Left;
			}
			else if (Dir >= -45 && Dir < 45)
			{
				return EArcaneMoveDirection::Forward;
			}
			else if (Dir >= 45 && Dir < 135)
			{
				return EArcaneMoveDirection::Right;
			}
		}
	}
	return EArcaneMoveDirection::None;
}

AActor* UArcaneBlueprintFunctionLibrary::GetNearestEnemyInFrontOfCharacter(AActor* InActor, TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes, bool bShowDebugTrace, float MaxDistance, float MaxAngle, FVector TraceBoxSize)
{
	check(InActor);

	// 获取InActor的位置和前向向量
	const FVector ActorLocation = InActor->GetActorLocation();
	const FVector ActorForward = InActor->GetActorForwardVector();

	AArcaneHeroCharacter* HeroCharacter = Cast<AArcaneHeroCharacter>(InActor);
	// 查看是否有软锁定角色
	if (HeroCharacter && IsValid(HeroCharacter->SoftLockedTargetActor))
	{
		AArcaneCharacterBase* Character = Cast<AArcaneCharacterBase>(HeroCharacter->SoftLockedTargetActor);
		if (Character && Character->IsAlive())
		{
			// 如果软锁定角色存活，查看软锁定角色是否还在范围内（距离是否在MaxDistance)
			const FVector TargetLocation = Character->GetActorLocation();
			if (FVector::Dist(ActorLocation, TargetLocation) <= MaxDistance)
			{
				// 如果软锁定角色在范围内，直接返回软锁定角色
				return Character;
			}
			else
			{
				// 如果软锁定角色不在范围内，则清除软锁定角色
				HeroCharacter->SoftLockedTargetActor = nullptr;
			}
		}
		else
		{
			HeroCharacter->SoftLockedTargetActor = nullptr;
		}
	}

	AActor* NearestEnemy = nullptr;

	TArray<AActor*> AvailableTargetToLock;
	TArray<FHitResult> BoxTraceHitResults;
	
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		InActor,					// 获取角色
		ActorLocation,				// 从英雄角色获取位置
		ActorLocation + ActorForward * MaxDistance,		// 从英雄角色获取前方1000米的位置
		TraceBoxSize * 0.5f,		// 射线盒大小
		ActorForward.ToOrientationRotator(),		// 从英雄角色获取前方向量
		TargetObjectTypes,		// 指定检测目标的对象类型
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
			if (AArcaneCharacterBase* ArcaneCharacter = Cast<AArcaneCharacterBase>(HitActor))
			{
				// 检查是是否存活
				if (!ArcaneCharacter->IsAlive())
				{
					continue;	// 如果角色不存活，则跳过
				}
			}
			if (HitActor != InActor)
			{
				AvailableTargetToLock.AddUnique(HitActor);
			}
		}
	}

	float ClosestDistance = TNumericLimits<float>::Max();

	// 使用GameplayStatics的FindNearestActor函数来查找最近的Actor，该函数会返回最近的Actor和距离
	NearestEnemy = UGameplayStatics::FindNearestActor(ActorLocation, AvailableTargetToLock, ClosestDistance);

	HeroCharacter->SoftLockedTargetActor = NearestEnemy;	// 将最近的敌人设置为软锁定目标

	return NearestEnemy;	// 返回最近的敌人Actor
}

UArcaneGameInstance* UArcaneBlueprintFunctionLibrary::GetArcaneGameInstance(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			return World->GetGameInstance<UArcaneGameInstance>();
		}
	}
	return nullptr;
}

void UArcaneBlueprintFunctionLibrary::ArcaneCloseGame(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		const ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController();
		
		if (UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
		{
			// 关闭游戏前，如果是手柄控制，需要切换到鼠标键盘输入类型
			if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
			{
				CommonInputSubsystem->SetCurrentInputType(ECommonInputType::MouseAndKeyboard);
			}
		}

		// 通过Kismet库来关闭游戏
		UKismetSystemLibrary::QuitGame(World, nullptr, EQuitPreference::Quit, true);
		
	}
}

void UArcaneBlueprintFunctionLibrary::SaveCurrentGameDifficulty(EArcaneGameDifficulty InDifficulty)
{
	USaveGame* SaveGameObj = UGameplayStatics::CreateSaveGameObject(UArcaneSaveGame::StaticClass());
	if (UArcaneSaveGame* ArcaneSaveGame = Cast<UArcaneSaveGame>(SaveGameObj))
	{
		ArcaneSaveGame->LastSelectedGameDifficulty = InDifficulty;
		const bool bWasSaved = UGameplayStatics::SaveGameToSlot(ArcaneSaveGame, ArcaneGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);
		
		Debug::Print(bWasSaved ? TEXT("Game Difficulty Saved Successfully.") : TEXT("Failed to Save Game Difficulty."));
	
	}
}

void UArcaneBlueprintFunctionLibrary::SaveUnlockedGameDifficulties(EArcaneGameDifficulty InUnlockedDifficulty)
{
	USaveGame* SaveGameObj = UGameplayStatics::CreateSaveGameObject(UArcaneSaveGame::StaticClass());
	if (UArcaneSaveGame* ArcaneSaveGame = Cast<UArcaneSaveGame>(SaveGameObj))
	{
		// 需要进行比较，如果传入的难度低于等于已经解锁的难度，则不进行保存
		if (!ArcaneSaveGame->UnlockedGameDifficulties.Contains(InUnlockedDifficulty))
		{
			ArcaneSaveGame->UnlockedGameDifficulties.AddUnique(InUnlockedDifficulty);
			if (ArcaneSaveGame->UnlockedGameDifficulty < InUnlockedDifficulty)
			{
				ArcaneSaveGame->UnlockedGameDifficulty = InUnlockedDifficulty;
			}
			const bool bWasSaved = UGameplayStatics::SaveGameToSlot(ArcaneSaveGame, ArcaneGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);
		
			Debug::Print(bWasSaved ? TEXT("Unlocked Game Difficulties Saved Successfully.") : TEXT("Failed to Save Unlocked Game Difficulties."));
		}
	}
}

bool UArcaneBlueprintFunctionLibrary::TryLoadSavedGameDifficulty(EArcaneGameDifficulty& OutLoadedDifficulty)
{
	if (UGameplayStatics::DoesSaveGameExist(ArcaneGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0))
	{
		USaveGame* SaveGameObj = UGameplayStatics::LoadGameFromSlot(ArcaneGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);
		if (UArcaneSaveGame* ArcaneSaveGame = Cast<UArcaneSaveGame>(SaveGameObj))
		{
			OutLoadedDifficulty = ArcaneSaveGame->LastSelectedGameDifficulty;
			return true;
		}
	}
	
	return false;
}

bool UArcaneBlueprintFunctionLibrary::TryLoadSavedUnlockedGameDifficulty(EArcaneGameDifficulty& OutLoadedUnlockedDifficulty)
{
	if (UGameplayStatics::DoesSaveGameExist(ArcaneGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0))
	{
		USaveGame* SaveGameObj = UGameplayStatics::LoadGameFromSlot(ArcaneGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);
		if (UArcaneSaveGame* ArcaneSaveGame = Cast<UArcaneSaveGame>(SaveGameObj))
		{
			OutLoadedUnlockedDifficulty = ArcaneSaveGame->UnlockedGameDifficulty;
			return true;
		}
	}
	return false;
}

void UArcaneBlueprintFunctionLibrary::ToggleInputMode(const UObject* WorldContextObject, EArcaneInputMode InputMode)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (!PlayerController) return;

		FInputModeGameOnly GameOnlyMode;
		FInputModeUIOnly UIOnlyMode;

		switch (InputMode) {
		case EArcaneInputMode::GameOnly:
			PlayerController->SetInputMode(GameOnlyMode);
			PlayerController->bShowMouseCursor = false;
			break;
		case EArcaneInputMode::UIOnly:
			PlayerController->SetInputMode(UIOnlyMode);
			PlayerController->bShowMouseCursor = true;
			break;
		}
	}
}

TSoftClassPtr<UWidget_ActivatableBase> UArcaneBlueprintFunctionLibrary::GetFrontendSoftWidgetClassByTag(UPARAM(meta = (Categories = "Frontend.Widget")) FGameplayTag WidgetTag)
{
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();

	check(FrontendDeveloperSettings);
	checkf(FrontendDeveloperSettings->FrontendWidgetMap.Contains(WidgetTag), TEXT("WidgetTag %s not found in FrontendWidgetMap!"), *WidgetTag.ToString());

	return FrontendDeveloperSettings->FrontendWidgetMap.FindRef(WidgetTag);	// 返回对应的软类

}

TSoftObjectPtr<UTexture2D> UArcaneBlueprintFunctionLibrary::GetOptionsSoftImageByTag(FGameplayTag ImageTag)
{
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
	check(FrontendDeveloperSettings);
	checkf(FrontendDeveloperSettings->FrontendOptionsSoftImageMap.Contains(ImageTag), TEXT("ImageTag %s not found in FrontendOptionsSoftImageMap!"), *ImageTag.ToString());

	return FrontendDeveloperSettings->FrontendOptionsSoftImageMap.FindRef(ImageTag);	// 返回对应的软类
}



