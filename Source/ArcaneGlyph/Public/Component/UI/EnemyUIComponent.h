// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARCANEGLYPH_API UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEnemyUIComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
