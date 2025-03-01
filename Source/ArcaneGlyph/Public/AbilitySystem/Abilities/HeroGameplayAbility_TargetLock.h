// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneHeroGameplayAbility.h"
#include "HeroGameplayAbility_TargetLock.generated.h"

class UInputMappingContext;
class UArcaneWidgetBase;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UHeroGameplayAbility_TargetLock : public UArcaneHeroGameplayAbility
{
	GENERATED_BODY()

public:

protected:
	// Begin UGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// End UGameplayAbility interface

	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(float DeltaTime);

private:

	void TryLockTargetLock();	// 尝试锁定目标
	void GetAvailableTargetToLock();	// 获取可锁定的目标
	AActor* GetNearestTargetFromAvailable(const TArray<AActor*> InAvailableActors);	// 从可用目标中获取最近的目标
	void DrawTargetLockWidget();	// 绘制目标锁定小部件
	void SetTargetLockWidgetPosition();	// 设置目标锁定小部件位置
	void InitTargetLockMovement();	// 初始化目标锁定移动
	void ResetTargetLockMovement();	// 重置目标锁定移动
	void InitTargetLockInputMappingContext();	// 初始化目标锁定输入映射上下文
	void ResetTargetLockInputMappingContext();	// 重置目标锁定输入映射上下文

	void CancelTargetLockAbility();	// 取消目标锁定能力
	void Cleanup();
	
	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	float LockDistance = 5000.f;	// 锁定距离

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	FVector TraceBoxSize = FVector(2000.f, 2000.f, 2000.f);		// 射线盒大小

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetLockObjectTypes;	// 锁定目标的对象类型

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	bool bShowDebugTrace = false;	// 是否显示调试射线

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	TSubclassOf<UArcaneWidgetBase> TargetLockWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	float TargetLockRotationInterpSpeed = 5.f;	// 目标锁定旋转插值速度

	UPROPERTY()
	FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;

	UPROPERTY()
	TObjectPtr<UArcaneWidgetBase> TargetLockWidget;

	UPROPERTY()
	TArray<AActor*> AvailableTargetToLock;	// 可锁定的目标

	UPROPERTY()
	AActor* CurrentLockedActor = nullptr;	// 锁定的目标

	UPROPERTY()
	float CachedDefaultMaxWalkSpeed = 0.f;	// 缓存的默认最大步行速度

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	float TargetLockMaxWalkSpeed = 200.f;	// 目标锁定时的最大步行速度

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	TObjectPtr<UInputMappingContext> TargetLockInputMappingContext;
	
};

