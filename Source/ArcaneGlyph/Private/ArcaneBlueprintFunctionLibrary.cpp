// INVI_1998 All Rights Reserved.


#include "ArcaneBlueprintFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArcaneDebugHelper.h"
#include "ArcaneGameplayTags.h"
#include "GenericTeamAgentInterface.h"
#include "KismetAnimationLibrary.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "Characters/ArcaneHeroCharacter.h"
#include "Component/Combat/PawnCombatComponent.h"
#include "Component/UI/PawnUIComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"
#include "Kismet/KismetMathLibrary.h"


UArcaneAbilitySystemComponent* UArcaneBlueprintFunctionLibrary::NativeGetArcaneASCFromActor(AActor* InActor)
{
	check(InActor);

	// 只要InActor实现了 IAbilitySystemInterface 接口的 GetAbilitySystemComponent函数，就可以获取到AbilitySystemComponent
	return CastChecked<UArcaneAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
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

EArcaneMoveDirection UArcaneBlueprintFunctionLibrary::GetMoveDirection(EArcaneMovementDirectionMethod Method, const FVector& InputVector, float DeadZone)
{
	const FVector SafeInput = InputVector.GetSafeNormal2D();
    if (SafeInput.IsNearlyZero(DeadZone)) return EArcaneMoveDirection::None;

    switch (Method)
    {
    case EArcaneMovementDirectionMethod::AxisDominant:
        // 基于分量绝对值比较的4方向判定
        return (FMath::Abs(SafeInput.X) > FMath::Abs(SafeInput.Y)) ?
            ((SafeInput.X > 0) ? EArcaneMoveDirection::Backward : EArcaneMoveDirection::Forward) :
            ((SafeInput.Y > 0) ? EArcaneMoveDirection::Left : EArcaneMoveDirection::Right);

    case EArcaneMovementDirectionMethod::AngleThreshold4:
        // 基于角度阈值分割的4方向判定
        {
            const float Angle = FMath::RadiansToDegrees(FMath::Atan2(SafeInput.Y, SafeInput.X));
            const float WrappedAngle = (Angle < 0) ? Angle + 360 : Angle;

            if (WrappedAngle >= 315 || WrappedAngle < 45)    return EArcaneMoveDirection::Backward;
            if (WrappedAngle >= 45 && WrappedAngle < 135)    return EArcaneMoveDirection::Left;
            if (WrappedAngle >= 135 && WrappedAngle < 225)   return EArcaneMoveDirection::Forward;
            return EArcaneMoveDirection::Right;
        }

    case EArcaneMovementDirectionMethod::AngleThreshold8:
        // 精确的8方向判定
        {
            constexpr float SectorSize = 45.0f; // 每个方向45度范围
            const float Angle = FMath::RadiansToDegrees(FMath::Atan2(SafeInput.Y, SafeInput.X));
            const float NormalizedAngle = FMath::Fmod(Angle + 360 + 22.5f, 360); // 偏移22.5度对齐中心

            const int32 SectorIndex = FMath::FloorToInt(NormalizedAngle / SectorSize);
            switch (SectorIndex)
            {
                case 0:  return EArcaneMoveDirection::Backward;
                case 1:  return EArcaneMoveDirection::BackwardLeft;
                case 2:  return EArcaneMoveDirection::Left;
                case 3:  return EArcaneMoveDirection::ForwardLeft;
                case 4:  return EArcaneMoveDirection::Forward;
                case 5:  return EArcaneMoveDirection::ForwardRight;
                case 6:  return EArcaneMoveDirection::Right;
                case 7:  return EArcaneMoveDirection::BackwardRight;
                default: return EArcaneMoveDirection::None;
            }
        }
	default: break;
    }
    return EArcaneMoveDirection::None;
}



