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
class ARCANEGLYPH_API UArcaneHeroStealthAbility : public UArcaneHeroGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Stealth Ability")
	TSubclassOf<APlayerPhantom> PhantomClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stealth Ability")
	APlayerPhantom* SpawnedPhantom = nullptr;
	
	UPROPERTY(BlueprintReadWrite, Category = "Stealth Ability")
	FTimerHandle PhantomDestroyTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stealth Ability")
	float PhantomLifetime = 15.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Stealth Ability")
	FGenericTeamId OriginalTeamID;
	

};
