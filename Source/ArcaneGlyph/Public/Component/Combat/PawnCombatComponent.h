// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Component/PawnExtensionComponentBase.h"
#include "PawnCombatComponent.generated.h"


class AArcaneWeaponBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARCANEGLYPH_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Arcane|Combat")
	void RegisterSpawnedWeapon(const FGameplayTag& InWeaponTag, AArcaneWeaponBase* InWeapon, bool bEquipped = false);

	UFUNCTION(BlueprintCallable, Category="Arcane|Combat")
	AArcaneWeaponBase* GetCharacterCarriedWeapon(const FGameplayTag& InWeaponTag) const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Arcane|Combat")
	FGameplayTag CurrentEquippedWeaponTag;		// 当前装备的武器标签

	UFUNCTION(BlueprintCallable, Category="Arcane|Combat")
	AArcaneWeaponBase* GetCharacterCurrentEquippedWeapon() const;
	
private:
	TMap<FGameplayTag, AArcaneWeaponBase*> CharacterCarriedWeaponMap;		// 角色携带的武器映射表

	
};
