// INVI_1998 All Rights Reserved.


#include "Component/Combat/EnemyCombatComponent.h"

#include "ArcaneDebugHelper.h"


void UEnemyCombatComponent::OnHitTargetActor(AActor* InHitActor)
{
	if (InHitActor)
	{
		Debug::Print(TEXT("EnemyCombatComponent::OnHitTargetActor: Hit Actor: " + InHitActor->GetName()));
	}
	
}
