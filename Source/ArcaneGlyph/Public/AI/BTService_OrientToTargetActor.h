// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_OrientToTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UBTService_OrientToTargetActor : public UBTService
{
	GENERATED_BODY()

	UBTService_OrientToTargetActor();

	// UBTNode interface begin
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;	// 从资产初始化
	virtual FString GetStaticDescription() const override;				// 获取静态描述
	// ~UBTNode interface end

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="Target")
	FBlackboardKeySelector InTargetActorKey;	// 目标角色键选择器

	UPROPERTY(EditAnywhere, Category="Target")
	float RotationInterpSpeed;	// 旋转插值速度

public:
	
};
