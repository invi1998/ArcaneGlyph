// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArcaneWeaponBase.generated.h"

class UBoxComponent;

UCLASS()
class ARCANEGLYPH_API AArcaneWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AArcaneWeaponBase();

	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh.Get(); }
	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const { return WeaponCollisionBox.Get(); }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UBoxComponent> WeaponCollisionBox;
	
};
