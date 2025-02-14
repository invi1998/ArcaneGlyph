// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneWeaponBase.h"
#include "ArcaneTypes/ArcaneStructTypes.h"
#include "ArcaneHeroWeapon.generated.h"

UCLASS()
class ARCANEGLYPH_API AArcaneHeroWeapon : public AArcaneWeaponBase
{
	GENERATED_BODY()

public:
	AArcaneHeroWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FArcaneHeroWeaponData HeroWeaponData;

};
