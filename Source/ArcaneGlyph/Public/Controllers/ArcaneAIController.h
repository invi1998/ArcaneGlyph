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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> EnemyAIPerceptionComponent;	// 敌人 AI 感知组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAISenseConfig_Sight> AISenseConfig_Sight;					// AI 感知配置：视觉

	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

};
