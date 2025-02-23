// INVI_1998 All Rights Reserved.


#include "Controllers/ArcaneAIController.h"

#include "Navigation/CrowdFollowingComponent.h"


// 该Super构造函数中的SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent")是用于设置默认的寻路组件
// 因为默认的寻路组件是PathFollowingComponent，而我们需要的是CrowdFollowingComponent，所以需要在构造函数中设置默认的寻路组件
AArcaneAIController::AArcaneAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	if (UCrowdFollowingComponent* CrowdFollowingComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
	}
}



