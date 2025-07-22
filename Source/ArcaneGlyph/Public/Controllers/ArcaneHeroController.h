// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "ArcaneHeroController.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
/**
 * 
 */
UCLASS()
class ARCANEGLYPH_API AArcaneHeroController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AArcaneHeroController();

	// IGenericTeamAgentInterface Begin
	// 获取团队 ID
	virtual FGenericTeamId GetGenericTeamId() const override;
	// ~IGenericTeamAgentInterface End

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void SetupInputComponent() override;

private:
	FGenericTeamId HeroTeamID;	// 英雄团队 ID（玩家团队 ID）

	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem;

	void HandleInputDeviceChanged(ECommonInputType NewInputType);
	
	
};
