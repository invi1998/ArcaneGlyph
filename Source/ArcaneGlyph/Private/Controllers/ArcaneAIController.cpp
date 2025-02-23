// INVI_1998 All Rights Reserved.


#include "Controllers/ArcaneAIController.h"

#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


// 该Super构造函数中的SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent")是用于设置默认的寻路组件
// 因为默认的寻路组件是PathFollowingComponent，而我们需要的是CrowdFollowingComponent，所以需要在构造函数中设置默认的寻路组件
AArcaneAIController::AArcaneAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	if (UCrowdFollowingComponent* CrowdFollowingComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
	}

	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;		// 是否检测敌人：开启
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;		// 检测中立单位：关闭
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;		// 检测友军：关闭
	AISenseConfig_Sight->SightRadius = 5000.0f;		// 视野半径
	AISenseConfig_Sight->LoseSightRadius = 0.0f;		// 失去视野半径，设置为0表示永远不会失去视野
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.0f;		// 外围视野角度, 360度表示全方位视野
	

	EnemyAIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyAIPerceptionComponent"));
	EnemyAIPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);	// 配置感知
	EnemyAIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());	// 设置主要感知，这里设置为视觉感知
	// 当感知更新时（检测到敌人），就会调用 OnEnemyPerceptionUpdated 这个回调函数
	EnemyAIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AArcaneAIController::OnEnemyPerceptionUpdated);	// 添加感知更新事件
}

void AArcaneAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
}



