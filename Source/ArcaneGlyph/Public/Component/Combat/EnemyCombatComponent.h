﻿// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PawnCombatComponent.h"
#include "EnemyCombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARCANEGLYPH_API UEnemyCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	virtual void OnHitTargetActor(AActor* InHitActor, int32 InCollisionBoxIndex) override;		// 当击中目标Actor时调用

protected:
	virtual void ToggleBodyCollisionBoxCollision(bool bEnable, EToggleDamageType InToggleDamageType);
	
};
