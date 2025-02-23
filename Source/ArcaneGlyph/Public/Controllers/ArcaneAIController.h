// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "ArcaneAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class ARCANEGLYPH_API AArcaneAIController : public AAIController
{
	GENERATED_BODY()

public:
	AArcaneAIController(const FObjectInitializer& ObjectInitializer);

	// IGenericTeamAgentInterface Begin
	// 获取团队态度，用于判断敌人和友军
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	// ~IGenericTeamAgentInterface End

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> EnemyAIPerceptionComponent;	// 敌人 AI 感知组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAISenseConfig_Sight> AISenseConfig_Sight;					// AI 感知配置：视觉

	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	UPROPERTY(EditDefaultsOnly, Category="Detor Crowd Avoidance Config")
	bool bEnableCrowdAvoidance = true;	// 是否启用人群避让

	// meta = (EditCondition = "bEnableCrowdAvoidance")) 意味着只有当 bEnableCrowdAvoidance 为 true 时，才会显示这个属性（即只有在启用人群避让时，才会显示这个属性，才能进行该项设置）
	// , UIMin = "1", UIMax = "4" 则是设置了该属性的最小值和最大值，让我能在编辑器中可以像滑动条一样调整这个整数值，进而控制人群避让的质量
	UPROPERTY(EditDefaultsOnly, Category="Detor Crowd Avoidance Config", meta = (EditCondition = "bEnableCrowdAvoidance", UIMin = "1", UIMax = "4"))
	int32 DetourCrowdAvoidanceQuality = 4;	// 人群避让质量

	UPROPERTY(EditDefaultsOnly, Category="Detor Crowd Avoidance Config", meta = (EditCondition = "bEnableCrowdAvoidance"))
	float CollisionQueryRange = 600.0f;		// 人群避让范围
	
	
};
