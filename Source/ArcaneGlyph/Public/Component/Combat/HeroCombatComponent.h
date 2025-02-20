// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PawnCombatComponent.h"
#include "HeroCombatComponent.generated.h"


class AArcaneHeroWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARCANEGLYPH_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Arcane|Combat")
	AArcaneHeroWeapon* GetHeroCarriedWeaponByTag(const FGameplayTag& InWeaponTag) const;

	UFUNCTION(BlueprintCallable, Category="Arcane|Combat")
	AArcaneHeroWeapon* GetHeroCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category="Arcane|Combat")
	float GetHeroCurrenEquippedWeaponDamageAtLevel(float InLevel) const;

	virtual void OnHitTargetActor(AActor* InHitActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InHitActor) override;
	
};
