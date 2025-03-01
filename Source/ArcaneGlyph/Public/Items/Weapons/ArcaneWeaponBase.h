// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArcaneWeaponBase.generated.h"

class UBoxComponent;

DECLARE_DELEGATE_TwoParams(FOnTargetInteractedDelegate, AActor*, int32);

UCLASS()
class ARCANEGLYPH_API AArcaneWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AArcaneWeaponBase();

	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh.Get(); }

	// 这里添加一个蓝图原生事件，用于某些特殊武器在蓝图里面实现特殊的逻辑（比如双头武器，有两个碰撞盒，但是另一个碰撞盒子是在蓝图里添加的，C++无法直接获取，所以需要蓝图实现）
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Weapon")
	void ToggleWeaponCollision(bool bEnable);

	FOnTargetInteractedDelegate OnWeaponHitTarget;		// 武器击中目标委托
	FOnTargetInteractedDelegate OnWeaponPulledTarget;		// 武器结束击中目标委托

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UBoxComponent> WeaponCollisionBox1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")
	bool bShouldUseTwoCollisionBoxes = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UBoxComponent> WeaponCollisionBox2;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void OnWeaponCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void OnWeaponCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
