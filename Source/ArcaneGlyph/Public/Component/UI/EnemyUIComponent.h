// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"


class UArcaneWidgetBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARCANEGLYPH_API UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyDrawnWidget(UArcaneWidgetBase* InEnemyDrawnWidget);

	UFUNCTION(BlueprintCallable)
	void RemoveAllEnemyDrawnWidget();

private:
	UPROPERTY()
	TArray<UArcaneWidgetBase*> EnemyDrawnWidgets;		// 绘制的敌人小部件
	
};
