// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcaneHeroFreezeGameplayAbility.h"
#include "GenericTeamAgentInterface.h"
#include "ArcaneHeroStealthAbility.generated.h"

class APlayerPhantom;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API UArcaneHeroStealthAbility : public UArcaneHeroFreezeGameplayAbility
{
	GENERATED_BODY()

public:
	UArcaneHeroStealthAbility();

	UPROPERTY(EditDefaultsOnly, Category = "Stealth Ability")
	TSubclassOf<APlayerPhantom> PhantomClass;

	UFUNCTION(BlueprintCallable, Category = "Stealth Ability")
	void SpawnStealthPhantom();

private:
	UPROPERTY()
	APlayerPhantom* SpawnedPhantom = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Stealth Ability")
	float PhantomLifetime = 15.0f;

	FGenericTeamId OriginalTeamID;
	
};
