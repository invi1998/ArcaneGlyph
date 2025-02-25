// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToFaceTarget.generated.h"

struct FRotateToFaceTargetTaskMemory
{
	TWeakObjectPtr<APawn> OwningPawn;	// 拥有者
	TWeakObjectPtr<AActor> TargetActor;	// 需要面向的目标

	bool IsValid() const
	{
		return OwningPawn.IsValid() && TargetActor.IsValid();
	}

	void Reset()
	{
		OwningPawn.Reset();		// 重置拥有者
		TargetActor.Reset();	// 重置目标
	}
};

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UBTTask_RotateToFaceTarget : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_RotateToFaceTarget();

	// UBTTaskNode interface Begin
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;		// 从行为树资产初始化
	virtual uint16 GetInstanceMemorySize() const override;	// 获取实例内存大小
	virtual FString GetStaticDescription() const override;	// 获取静态描述
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;	// 执行任务
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;	// Tick任务
	// ~UBTTaskNode interface End

	bool HasReachedAnglePrecision(APawn* QueryPawn, AActor* TargetActor) const;	// 是否达到角度精度

	UPROPERTY(EditAnywhere, Category="Face Target")
	float AnglePrecision = 10.f;	// 角度精度

	UPROPERTY(EditAnywhere, Category="Face Target")
	float RotationInterpSpeed = 5.f;	// 旋转插值速度

	UPROPERTY(EditAnywhere, Category="Face Target")
	FBlackboardKeySelector TargetToFaceKey;	// 需要面向的目标
};
