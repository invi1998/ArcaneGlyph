// INVI_1998 All Rights Reserved.

#pragma once

#include "ArcaneStructTypes.generated.h"

class UArcaneHeroLinkedAnimLayer;
class AArcaneWeaponBase;

USTRUCT(BlueprintType)
struct FArcaneHeroWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	TSubclassOf<UArcaneHeroLinkedAnimLayer> WeaponAnimLayerToLink;		// 链接的动画层
	
};
