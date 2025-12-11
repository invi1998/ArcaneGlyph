// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "GenericTeamAgentInterface.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "ArcaneHeroController.generated.h"

class UWidget_ActivatableBase;
class UInputAction;
class UInputMappingContext;
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
	// 设置团队 ID
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override { HeroTeamID = TeamID; }
	// ~IGenericTeamAgentInterface End
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* GameMenuMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* OpenGameMenuAction;
	
	UFUNCTION(BlueprintCallable, Category = "Arcane | HeroController")
	void SetGameMenuOpen(bool bIsOpen);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OpenGameMenu();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	void ToggleGameMenu(const FInputActionValue& InputActionValue);
	
	virtual void SetupInputComponent() override;

private:
	FGenericTeamId HeroTeamID;	// 英雄团队 ID（玩家团队 ID）

	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem;

	void HandleInputDeviceChanged(ECommonInputType NewInputType);
	
	bool bGameMenuOpen = false;
	
};
