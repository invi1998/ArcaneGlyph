// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArcaneHeroController.generated.h"

struct FInputActionValue;
class UDadaAsset_InputConfig;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API AArcaneHeroController : public APlayerController
{
	GENERATED_BODY()

public:
	AArcaneHeroController();

	virtual void SetupInputComponent() override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDadaAsset_InputConfig> InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Jump();
	
};
