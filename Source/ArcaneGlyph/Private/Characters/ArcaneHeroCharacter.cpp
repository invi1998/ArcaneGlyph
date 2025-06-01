// INVI_1998 All Rights Reserved.


#include "Characters/ArcaneHeroCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "AnimInstances/Hero/ArcaneHeroAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Component/Combat/HeroCombatComponent.h"
#include "Component/Input/ArcaneInputComponent.h"
#include "Component/UI/HeroUIComponent.h"
#include "DataAssets/StartupData/DataAsset_HeroStartupDada.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/ArcaneGaitDataInterface.h"


AArcaneHeroCharacter::AArcaneHeroCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 450.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 55.0f, 65.0f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;		// 角色面向移动方向
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);		// 角色旋转速度
	
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


	if (UArcaneInputComponent* EnhancedInputComponent = CastChecked<UArcaneInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, ArcaneGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &AArcaneHeroCharacter::Input_Move);
		EnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, ArcaneGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &AArcaneHeroCharacter::Input_Look);

		EnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, ArcaneGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Triggered, this, &AArcaneHeroCharacter::Input_SwitchTargetTriggered);
		EnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, ArcaneGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Completed, this, &AArcaneHeroCharacter::Input_SwitchTargetCompleted);
		
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


void AArcaneHeroCharacter::UpdateGait(EArcaneGaits InNewGait)
{
	PreviousGait = CurrentGait;
	CurrentGait = InNewGait;

	if (FArcaneGaitSetting* GaitSetting = ArcaneGaits.Find(CurrentGait))
	{
		GetCharacterMovement()->MaxWalkSpeed = GaitSetting->MaxWalkSpeed;
		GetCharacterMovement()->MaxAcceleration = GaitSetting->MaxAcceleration;
		GetCharacterMovement()->BrakingDecelerationWalking = GaitSetting->BreakingDeceleration;
		GetCharacterMovement()->BrakingFrictionFactor = GaitSetting->BrakingFrictionFactor;
		GetCharacterMovement()->bUseSeparateBrakingFriction = GaitSetting->bUseSeparateBrakingFriction;
		GetCharacterMovement()->BrakingFriction = GaitSetting->BreakFriction;
		GetCharacterMovement()->JumpZVelocity = GaitSetting->MaxJumpHeight;
	}

	if (UArcaneHeroAnimInstance* AnimInstance = Cast<UArcaneHeroAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		// 判断AnimInstance是否实现了U
		if (IArcaneGaitDataInterface* GaitDataInterface = AnimInstance->Implements<UArcaneGaitDataInterface>() ? Cast<IArcaneGaitDataInterface>(AnimInstance) : nullptr)
		{
			// 调用接口函数
			IArcaneGaitDataInterface::Execute_ReceiveGaitData(AnimInstance, CurrentGait);
		}
	}

	
	
}

bool AArcaneHeroCharacter::IsCanDoubleJump() const
{
	return GetCharacterMovement()->CanAttemptJump();
}

bool AArcaneHeroCharacter::CharacterJumpState(EArcaneJumpStateType& JumpState)
{
	JumpState = EArcaneJumpStateType::OnGround;
	if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		// 然后根据角色的移动速度判定是否下落
		if (GetCharacterMovement()->Velocity.Z <= 0.f)
		{
			// 角色正在下落
			JumpState = EArcaneJumpStateType::Falling;
		}
		if (GetCharacterMovement()->Velocity.Z > 0.f && JumpCurrentCount > 0)
		{
			// 角色正在上升
			JumpState = EArcaneJumpStateType::Jumping;
		}
	}
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		// 角色在地面上
		JumpState = EArcaneJumpStateType::OnGround;
	}

	return true;
}

void AArcaneHeroCharacter::DoubleJump()
{
	LaunchCharacter(FVector(0.f, 0.f, DoubleJumpZVelocity), false, true);
	JumpCurrentCount++;
}

void AArcaneHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateGait(EArcaneGaits::Jogging);
	
}

void AArcaneHeroCharacter::ProcessLockedMovement(float InputAxisValue)
{
	if (!CurrentLockedTargetActor) return;

	// 1. 计算目标方向
	FVector ToTarget = CurrentLockedTargetActor->GetActorLocation() - GetActorLocation();
	FVector RadialDir = ToTarget.GetSafeNormal();
	
	// 2. 确定切线方向（关键：根据输入符号调整）
	float InputSign = FMath::Sign(InputAxisValue);
	FVector TangentDir = FVector::CrossProduct(
		GetActorUpVector(), // 注意顺序：Up x Radial 得到左侧切线
		RadialDir
	).GetSafeNormal() * InputSign;

	// 3. 计算向心补偿角度
	float Speed = GetVelocity().Size();
	float Radius = ToTarget.Size();
	float CentripetalAngle = FMath::RadiansToDegrees(
		FMath::Atan((Speed * Speed) / (Radius * CentripetalFactor)) // 100为调试参数
	);

	// 4. 方向修正（左移顺时针补偿，右移逆时针补偿）
	FVector AdjustedDir = TangentDir.RotateAngleAxis(
		-InputSign * CentripetalAngle, // 关键符号处理
		GetActorUpVector()
	);
	AdjustedDir.Normalize();

	// 5. 应用修正后的移动方向
	AddMovementInput(AdjustedDir, FMath::Abs(InputAxisValue));
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
		// if (IsValid(CurrentLockedTargetActor))
		// {
		// 	ProcessLockedMovement(AxisValue.X);
		// }
		// else
		// {
		// 	AddMovementInput(RightDirection, AxisValue.X);
		// }
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
	if (GetCharacterMovement()->IsFalling())
	{
		// 如果当前角色正在跳跃，则执行二段跳
		if (GetCharacterMovement()->CanAttemptJump())
		{
			LaunchCharacter(FVector(0.f, 0.f, DoubleJumpZVelocity), false, true);
		}
	}
	else
	{
		// 如果当前角色没有在跳跃，则执行普通跳跃
		ACharacter::Jump();
	}
}

void AArcaneHeroCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	// 保存该输入信息，以便在输入动作（鼠标移动动作）完成后进行事件判定
	SwitchDirection = InputActionValue.Get<FVector2d>();
}

void AArcaneHeroCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData EventData;

	// 如果鼠标输入的X轴大于0，说明鼠标右移动，所以发送向右切换锁定事件，同理，左边。
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		SwitchDirection.X > 0.f ? ArcaneGameplayTags::Player_Event_SwitchLockTarget_Right : ArcaneGameplayTags::Player_Event_SwitchLockTarget_Left,
		EventData
	);
}

void AArcaneHeroCharacter::Input_AbilityPressed(FGameplayTag InInputTag)
{
	ArcaneAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AArcaneHeroCharacter::Input_AbilityReleased(FGameplayTag InInputTag)
{
	ArcaneAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}


