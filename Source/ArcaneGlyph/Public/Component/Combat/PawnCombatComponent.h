// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Component/PawnExtensionComponentBase.h"
#include "PawnCombatComponent.generated.h"


class AArcaneWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand,
	Head
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARCANEGLYPH_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Arcane|Combat")
	void RegisterSpawnedWeapon(const FGameplayTag& InWeaponTag, AArcaneWeaponBase* InWeapon, bool bEquipped = false, bool bLeftHand = false);

	UFUNCTION(BlueprintCallable, Category="Arcane|Combat")
	AArcaneWeaponBase* GetCharacterCarriedWeapon(const FGameplayTag& InWeaponTag) const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Arcane|Combat")
	FGameplayTag CurrentEquippedLeftHandWeaponTag;		// 当前左手装备的武器标签

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Arcane|Combat")
	FGameplayTag CurrentEquippedRightHandWeaponTag;		// 当前右手装备的武器标签

	UFUNCTION(BlueprintCallable, Category="Arcane|Combat")
	AArcaneWeaponBase* GetCharacterCurrentEquippedWeapon(bool bLeftHand = false) const;

	UFUNCTION(BlueprintCallable, Category="Arcane|Combat")
	bool IsCharacterEquippedWeapon() const;
	
	// 这里添加一个蓝图原生事件，用于某些特殊武器在蓝图里面实现特殊的逻辑（比如双头武器，有两个碰撞盒，但是另一个碰撞盒子是在蓝图里添加的，C++无法直接获取，所以需要蓝图实现）
	UFUNCTION(BlueprintCallable, Category="Arcane|Combat")
	void ToggleWeaponCollision(bool bEnable, EToggleDamageType InToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	virtual void OnHitTargetActor(AActor* InHitActor, int32 InCollisionBoxIndex);
	virtual void OnWeaponPulledFromTargetActor(AActor* InHitActor, int32 InCollisionBoxIndex);

protected:
	UPROPERTY()
	TArray<AActor*> HitOverlappedActors;		// 击中的重叠的角色

	virtual void ToggleCurrentEquippedWeaponCollision(bool bEnable);
	virtual void ToggleBodyCollisionBoxCollision(bool bEnable, EToggleDamageType InToggleDamageType);
	
private:
	TMap<FGameplayTag, AArcaneWeaponBase*> CharacterCarriedWeaponMap;		// 角色携带的武器映射表

	
};


