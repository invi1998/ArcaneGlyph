// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneCharacterBase.h"
#include "ArcaneHeroCharacter.generated.h"

UCLASS()
class ARCANEGLYPH_API AArcaneHeroCharacter : public AArcaneCharacterBase
{
	GENERATED_BODY()

public:
	AArcaneHeroCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	
};
