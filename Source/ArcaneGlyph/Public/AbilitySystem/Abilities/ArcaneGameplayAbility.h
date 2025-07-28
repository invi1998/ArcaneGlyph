// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ArcaneTypes/ArcaneEnumTypes.h"
#include "ArcaneGameplayAbility.generated.h"

class UArcaneAbilitySystemComponent;
class UPawnCombatComponent;
// 定义法术能力的激活策略
UENUM()
enum class EArcaneAbilityActivationPolicy : uint8
{
	OnTriggered,		// 当法术被触发时激活
	OnGiven,			// 在给定的时间内激活
};

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	// ~UGameplayAbility Interface
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Arcane | Ability")
	EArcaneAbilityActivationPolicy AbilityActivationPolicy = EArcaneAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure, Category = "Arcane | Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Arcane | Ability")
	UArcaneAbilitySystemComponent* GetArcaneAbilitySystemComponentFromActorInfo() const;

	FActiveGameplayEffectHandle NativeApplyGameplayEffectSpecToTarget(AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle) const;

	UFUNCTION(BlueprintCallable, Category = "Arcane | Ability", meta = (DisplayName = "Apply GameplayEffect Spec Handle To Target", ExpandEnumAsExecs = "SuccessType"))
	FActiveGameplayEffectHandle BP_ApplyGameplayEffectSpecToTarget(AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EArcaneSuccessType& SuccessType);

	UFUNCTION(BlueprintCallable, Category = "Arcane | Ability")
	void ApplyGameplayEffectToHitResults(const TArray<FHitResult>& InHitResults, const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "Arcane | Ability")
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FName& SocketName, AActor* HomingTarget, bool bOverridePitch = false, float Pitch = 0.0f);

	UFUNCTION(BlueprintPure, Category = "Arcane | Ability")
	FGameplayEffectSpecHandle MakeRageCostOrGainEffectSpecHandle(TSubclassOf<UGameplayEffect> InGameplayEffectClass, bool bIsCost = true);

	// 传入技能Tag，传入气力消耗值，创建气力消耗的GESpecHandle
	UFUNCTION(BlueprintPure, Category = "Arcane | Ability")
	FGameplayEffectSpecHandle MakeEnergyCostEffectSpecHandle(TSubclassOf<UGameplayEffect> InGameplayEffectClass, FGameplayTag InAbilityTag, float InEnergyCostValue = 0.0f) const;

	// 传入技能Tag，传入蓝量消耗值，创建蓝量消耗的GESpecHandle
	UFUNCTION(BlueprintPure, Category = "Arcane | Ability")
	FGameplayEffectSpecHandle MakeManaCostEffectSpecHandle(TSubclassOf<UGameplayEffect> InGameplayEffectClass, FGameplayTag InAbilityTag, float InManaCostValue = 0.0f) const;
	
};
