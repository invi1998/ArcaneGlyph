// INVI_1998 All Rights Reserved.


#include "Widget/Component/ArcaneWidget_CommonButtonBase.h"

#include "Interfaces/PawnUIInterface.h"

void UArcaneWidget_CommonButtonBase::InitEnemyCreatedWidget(AActor* InEnemyActor)
{
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(InEnemyActor))
	{
		if (UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent())
		{
			BP_OnEnemyCreatedWidgetInitialized(EnemyUIComponent);
		}
	}
}

void UArcaneWidget_CommonButtonBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UHeroUIComponent* HeroUIComponent = PawnUIInterface->GetHeroUIComponent())
		{
			BP_OnOwningHeroUIComponentInitialized(HeroUIComponent);
		}
	}
}
