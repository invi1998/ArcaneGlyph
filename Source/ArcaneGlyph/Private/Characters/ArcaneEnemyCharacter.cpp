// INVI_1998 All Rights Reserved.


#include "Characters/ArcaneEnemyCharacter.h"

#include "Component/Combat/EnemyCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AArcaneEnemyCharacter::AArcaneEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;		// 自动拥有AI控制权
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;		// 角色面向移动方向
	GetCharacterMovement()->bUseControllerDesiredRotation = false;		// 不使用控制器期望旋转
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.0f, 0.0f);		// 角色旋转速度
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;		// 角色移动速度
	GetCharacterMovement()->BrakingDecelerationWalking = 1024.0f;		// 角色停止时的减速度

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EnemyCombatComponent"));
}

void AArcaneEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
