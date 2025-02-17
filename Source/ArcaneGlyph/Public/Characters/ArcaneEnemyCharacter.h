// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneCharacterBase.h"
#include "ArcaneEnemyCharacter.generated.h"

class UEnemyCombatComponent;

UCLASS()
class ARCANEGLYPH_API AArcaneEnemyCharacter : public AArcaneCharacterBase
{
	GENERATED_BODY()

public:
	AArcaneEnemyCharacter();

	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UEnemyCombatComponent> EnemyCombatComponent;

};
