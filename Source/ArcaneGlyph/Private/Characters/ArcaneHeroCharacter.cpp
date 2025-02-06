// INVI_1998 All Rights Reserved.


#include "Characters/ArcaneHeroCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


AArcaneHeroCharacter::AArcaneHeroCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 55.0f, 65.0f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;		// 角色面向移动方向
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);		// 角色旋转速度
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;		// 角色移动速度
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;		// 角色停止时的减速度
	GetCharacterMovement()->JumpZVelocity = 600.0f;		// 角色跳跃高度
}

void AArcaneHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}


void AArcaneHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


