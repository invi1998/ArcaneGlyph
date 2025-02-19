// INVI_1998 All Rights Reserved.


#include "Characters/ArcaneEnemyCharacter.h"

#include "Component/Combat/EnemyCombatComponent.h"
#include "DataAssets/StartupData/DataAsset_EnemyStartupDada.h"
#include "Engine/AssetManager.h"
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

UPawnCombatComponent* AArcaneEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent.Get();
}

void AArcaneEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArcaneEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitEnemyStartupData();
}

void AArcaneEnemyCharacter::InitEnemyStartupData()
{
	// 对于TSoftObjectPtr类型的数据，需要先加载数据，然后再使用
	// Enemy角色的生成，我们希望他是异步生成的
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartupData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this]()
		{
			if (UDataAsset_StartupDadaBase* LoadedData = CharacterStartupData.Get())
			{
				LoadedData->GiveToAbilitySystemComponent(ArcaneAbilitySystemComponent.Get());
			}
		})
	);
}
