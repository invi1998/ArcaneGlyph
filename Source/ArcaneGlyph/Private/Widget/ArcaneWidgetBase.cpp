// INVI_1998 All Rights Reserved.


#include "Widget/ArcaneWidgetBase.h"

#include "Component/UI/HeroUIComponent.h"
#include "Interfaces/PawnUIInterface.h"

void UArcaneWidgetBase::NativeOnInitialized()
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
