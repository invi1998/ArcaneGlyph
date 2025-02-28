// INVI_1998 All Rights Reserved.


#include "Characters/ArcaneHeroCharacter.h"

#include "ArcaneGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/Combat/HeroCombatComponent.h"
#include "Component/Input/ArcaneInputComponent.h"
#include "Component/UI/HeroUIComponent.h"
#include "DataAssets/StartupData/DataAsset_HeroStartupDada.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


AArcaneHeroCharacter::AArcaneHeroCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 55.0f, 65.0f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;		// 角色面向移动方向
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);		// 角色旋转速度
	GetCharacterMovement()->MaxWalkSpeed = 450.0f;		// 角色移动速度
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;		// 角色停止时的减速度
	GetCharacterMovement()->JumpZVelocity = 600.0f;		// 角色跳跃高度

	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));
	HeroUIComponent = CreateDefaultSubobject<UHeroUIComponent>(TEXT("HeroUIComponent"));
}

void AArcaneHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		EnhancedInputSubsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
	}
	

	UArcaneInputComponent* EnhancedInputComponent = CastChecked<UArcaneInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, ArcaneGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &AArcaneHeroCharacter::Input_Move);
		EnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, ArcaneGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &AArcaneHeroCharacter::Input_Look);
		EnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, ArcaneGameplayTags::InputTag_Jump, ETriggerEvent::Triggered, this, &AArcaneHeroCharacter::Input_Jump);

		EnhancedInputComponent->BindAbilityInputAction(InputConfigDataAsset,this, &AArcaneHeroCharacter::Input_AbilityPressed, &AArcaneHeroCharacter::Input_AbilityReleased);
	}
	
}

void AArcaneHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 对于TSoftObjectPtr类型的数据，需要先加载数据，然后再使用
	// IsValid()函数用于判断TSoftObjectPtr是否有效, IsNull()函数用于判断TSoftObjectPtr是否为空
	if (!CharacterStartupData.IsNull())
	{
		// 对于角色的初始化数据，我们需要在角色被控制器控制时加载数据，我希望这个过程是同步的，因此使用LoadSynchronous()函数
		if (UDataAsset_StartupDadaBase* LoadedData = CharacterStartupData.LoadSynchronous())
		{
			// 这里可以使用加载到的数据进行初始化
			LoadedData->GiveToAbilitySystemComponent(ArcaneAbilitySystemComponent.Get());
		}
	}
	
}

UPawnCombatComponent* AArcaneHeroCharacter::GetPawnCombatComponent() const
{
	return HeroCombatComponent;
}

UPawnUIComponent* AArcaneHeroCharacter::GetPawnUIComponent() const
{
	return HeroUIComponent;
}

UHeroUIComponent* AArcaneHeroCharacter::GetHeroUIComponent() const
{
	return HeroUIComponent;
}


void AArcaneHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArcaneHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2d AxisValue = InputActionValue.Get<FVector2d>();
	const FRotator CurrentControlRotation = GetControlRotation();
	const FRotator YawRotation(0.0f, CurrentControlRotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (AxisValue.X != 0.0f)
	{
		AddMovementInput(RightDirection, AxisValue.X);
	}
	
	if (AxisValue.Y != 0.0f)
	{
		AddMovementInput(ForwardDirection, AxisValue.Y);
	}
}

void AArcaneHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2d AxisValue = InputActionValue.Get<FVector2d>();
	if (AxisValue.X != 0.f)
	{
		AddControllerYawInput(AxisValue.X);
	}
	
	if (AxisValue.Y != 0.f)
	{
		AddControllerPitchInput(AxisValue.Y);
	}
}

void AArcaneHeroCharacter::Input_Jump()
{
}

void AArcaneHeroCharacter::Input_AbilityPressed(FGameplayTag InInputTag)
{
	ArcaneAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AArcaneHeroCharacter::Input_AbilityReleased(FGameplayTag InInputTag)
{
	ArcaneAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}


