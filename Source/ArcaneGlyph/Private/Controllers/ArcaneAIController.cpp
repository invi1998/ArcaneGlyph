// INVI_1998 All Rights Reserved.


#include "Controllers/ArcaneAIController.h"

#include "ArcaneDebugHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


// 该Super构造函数中的SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent")是用于设置默认的寻路组件
// 因为默认的寻路组件是PathFollowingComponent，而我们需要的是CrowdFollowingComponent，所以需要在构造函数中设置默认的寻路组件
AArcaneAIController::AArcaneAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
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

	AAIController::SetGenericTeamId(FGenericTeamId(1));	// 设置团队 ID: 1, 用于区分敌人和友军,团队 ID 为 0 表示友军，团队 ID 为 1 表示敌人

}

ETeamAttitude::Type AArcaneAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	// 如果 Other 是一个 Pawn
	if (const APawn* PawnToCheck = Cast<APawn>(&Other))
	{
		// 如果 PawnToCheck 有一个 IGenericTeamAgentInterface 接口
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(PawnToCheck->GetController()))
		{
			// return Super::GetTeamAttitudeTowards(*PawnToCheck->GetController());

			// 如果 PawnToCheck 的团队 ID 小于当前控制器的团队 ID, 则返回敌对态度(因为只有玩家的团队 ID 为 0，而敌人的团队 ID 为 1，而其他未被设置团队 ID 的 Actor 的团队 ID 为 255)
			if (TeamAgent->GetGenericTeamId() < GetGenericTeamId())
			{
				return ETeamAttitude::Hostile;
			}
		}
	}

	return ETeamAttitude::Friendly;
}

void AArcaneAIController::BeginPlay()
{
	Super::BeginPlay();

	if (UCrowdFollowingComponent* CrowdFollowingComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		// 是否启用人群避让
		CrowdFollowingComp->SetCrowdSimulationState(bEnableCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::ObstacleOnly);

		// 设置人群避让质量
		switch (DetourCrowdAvoidanceQuality)
		{
		case 1:
			CrowdFollowingComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);
			break;
		case 2:
			CrowdFollowingComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium);
			break;
		case 3:
			CrowdFollowingComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);
			break;
		case 4:
			CrowdFollowingComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
			break;
		default: break;
		}

		// 设置避让组：1表示我们当前的AI应该对其他AI（EnemyAI，因为我们此前设置的团队ID 1 就是我们的EnemyTeam)进行避让
		CrowdFollowingComp->SetAvoidanceGroup(1);				// 设置避让组
		CrowdFollowingComp->SetGroupsToAvoid(1);		// 设置需要避让的组

		// 设置人群避让半径
		CrowdFollowingComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
		
	}
}

void AArcaneAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		if (!BlackboardComp->GetValueAsObject(FName("TargetActor")))
		{
			if (Stimulus.WasSuccessfullySensed() && Actor)
			{
				BlackboardComp->SetValueAsObject(FName("TargetActor"), Actor);
			}
		}
	}
}



