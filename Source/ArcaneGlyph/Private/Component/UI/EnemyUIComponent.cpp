// INVI_1998 All Rights Reserved.


#include "Component/UI/EnemyUIComponent.h"

#include "Widget/ArcaneWidgetBase.h"


void UEnemyUIComponent::RegisterEnemyDrawnWidget(UArcaneWidgetBase* InEnemyDrawnWidget)
{
	EnemyDrawnWidgets.Add(InEnemyDrawnWidget);
}

void UEnemyUIComponent::RemoveAllEnemyDrawnWidget()
{
	for (UArcaneWidgetBase* EnemyDrawnWidget : EnemyDrawnWidgets)
	{
		if (EnemyDrawnWidget)
		{
			EnemyDrawnWidget->RemoveFromParent();
		}
	}

	EnemyDrawnWidgets.Empty();
}
