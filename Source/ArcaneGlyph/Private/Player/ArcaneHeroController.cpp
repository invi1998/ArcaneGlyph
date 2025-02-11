// INVI_1998 All Rights Reserved.


#include "Player/ArcaneHeroController.h"

#include "ArcaneGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "Component/Input/ArcaneInputComponent.h"
#include "DataAssets/Input/DadaAsset_InputConfig.h"

AArcaneHeroController::AArcaneHeroController()
{
}

void AArcaneHeroController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UArcaneInputComponent* EnhancedInputComponent = CastChecked<UArcaneInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, ArcaneGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &AArcaneHeroController::Input_Move);
		EnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, ArcaneGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &AArcaneHeroController::Input_Look);
		EnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, ArcaneGameplayTags::InputTag_Jump, ETriggerEvent::Triggered, this, &AArcaneHeroController::Input_Jump);
	}
}

void AArcaneHeroController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (EnhancedInputSubsystem)
	{
		EnhancedInputSubsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
	}
	
}

void AArcaneHeroController::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2d AxisValue = InputActionValue.Get<FVector2d>();
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, AxisValue.X);
		ControlledPawn->AddMovementInput(RightDirection, AxisValue.Y);
	}
}

void AArcaneHeroController::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2d AxisValue = InputActionValue.Get<FVector2d>();
	AddYawInput(AxisValue.X);
	AddPitchInput(AxisValue.Y);
}

void AArcaneHeroController::Input_Jump()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		// ControlledPawn->Jump();
	}
}
