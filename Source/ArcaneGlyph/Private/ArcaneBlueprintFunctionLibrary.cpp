// INVI_1998 All Rights Reserved.


#include "ArcaneBlueprintFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArcaneDebugHelper.h"
#include "ArcaneGameplayTags.h"
#include "GenericTeamAgentInterface.h"
#include "KismetAnimationLibrary.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "Component/Combat/PawnCombatComponent.h"
#include "Component/UI/PawnUIComponent.h"
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
		return PawnCombatComponent->CurrentEquippedWeaponTag.IsValid();
	}
	return false;
}


