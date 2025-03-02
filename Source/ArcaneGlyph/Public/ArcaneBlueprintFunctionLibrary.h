﻿// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ArcaneTypes/ArcaneEnumTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ArcaneBlueprintFunctionLibrary.generated.h"

struct FGameplayEffectSpecHandle;
class AArcaneHeroCharacter;
struct FScalableFloat;
class UPawnUIComponent;
class UPawnCombatComponent;
class UArcaneAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UArcaneAbilitySystemComponent* NativeGetArcaneASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Arcane | FunctionLibrary")
	static void AddGameplayTagToActorIfNotHas(AActor* InActor, FGameplayTag InTag);

	UFUNCTION(BlueprintCallable, Category = "Arcane | FunctionLibrary")
	static void RemoveGameplayTagFromActorIfHas(AActor* InActor, FGameplayTag InTag);

	static bool NativeDoesActorHasGameplayTag(AActor* InActor, FGameplayTag InTag);

	// DisplayName这个元属性可以用来指定在蓝图中显示的函数名，ExpandEnumAsExecs这个元属性可以用来指定枚举类型作为执行枚举类型,这样在蓝图中就可以使用枚举类型作为执行枚举类型
	// 换句话说，就是我们可以直接将结果枚举类型作为蓝图节点的执行后的分支引脚（就类似PlayMontageAndWaitForEvent节点里的那个动画事件枚举类型）
	UFUNCTION(BlueprintCallable, Category = "Arcane | FunctionLibrary", meta=(DisplayName="Does Actor Has GameplayTag", ExpandEnumAsExecs="ConfirmType"))
	static void BP_DoesActorHasGameplayTag(AActor* InActor, FGameplayTag InTag, EArcaneConfirmType& ConfirmType);
	
	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Arcane | FunctionLibrary", meta=(DisplayName="Get Pawn Combat Component From Actor", ExpandEnumAsExecs="ValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EArcaneValidType& ValidType);

	static UPawnUIComponent* NativeGetPawnUIComponentFromActor(AActor* InActor);

	// 查询目标是否为敌对
	UFUNCTION(BlueprintPure, Category = "Arcane | FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* InQueryPawn, APawn* InTargetPawn);

	// 向Actor的UI组件广播GameplayTag改变事件
	UFUNCTION(BlueprintCallable, Category = "Arcane | FunctionLibrary")
	static void BroadcastGameplayTagChangedToUIComponent(AActor* InActor, FGameplayTag InTag, bool bAddTag);

	// 获取指定等级的属性值
	UFUNCTION(BlueprintPure, Category = "Arcane | FunctionLibrary", meta=(CompactNodeTitle="Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, int32 InLevel);

	// 计算击中反应方向标签
	UFUNCTION(BlueprintPure, Category = "Arcane | FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAndleDifference);

	// 获取角色是否装备了武器
	UFUNCTION(BlueprintPure, Category = "Arcane | FunctionLibrary")
	static bool IsCharacterEquippedWeapon(AActor* InActor);

	// 设置角色最大步行速度
	UFUNCTION(BlueprintCallable, Category = "Arcane | FunctionLibrary")
	static void SetCharacterMaxWalkSpeed(AActor* InActor, float InMaxWalkSpeed);

	// 获取角色最大步行速度
	UFUNCTION(BlueprintPure, Category = "Arcane | FunctionLibrary")
	static float GetCharacterMaxWalkSpeed(AActor* InActor);

	// 判定当前格挡是否是有效的
	UFUNCTION(BlueprintPure, Category = "Arcane | FunctionLibrary")
	static bool IsCurrentBlockValid(AActor* InAttacker, AActor* InDefender);

	// 设置玩家是否是左手正持有武器
	UFUNCTION(BlueprintCallable, Category = "Arcane | FunctionLibrary")
	static void SetPlayerIsLeftHandUsingWeapon(AActor* InActor, bool bIsLeftHandUsingWeapon);

	UFUNCTION(BlueprintCallable, Category = "Arcane | FunctionLibrary")
	static bool ApplyGameplayEffectSpecHandleToTarget(AActor* InInstigator, AActor* InTarget, const FGameplayEffectSpecHandle& InEffectSpecHandle);
	
};


