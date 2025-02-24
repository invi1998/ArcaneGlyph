// INVI_1998 All Rights Reserved.


#include "ArcaneBlueprintFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArcaneDebugHelper.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"


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
