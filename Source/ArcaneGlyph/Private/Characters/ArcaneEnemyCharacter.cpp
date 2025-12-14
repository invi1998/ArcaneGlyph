// INVI_1998 All Rights Reserved.


#include "Characters/ArcaneEnemyCharacter.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneDebugHelper.h"
#include "Component/Combat/EnemyCombatComponent.h"
#include "Component/UI/EnemyUIComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Controllers/ArcaneAIController.h"
#include "DataAssets/StartupData/DataAsset_EnemyStartupDada.h"
#include "Engine/AssetManager.h"
#include "Game/ArcaneGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widget/ArcaneWidgetBase.h"


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
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>(TEXT("EnemyUIComponent"));

	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthWidgetComponent"));
	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());
	EnemyHealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	
}

UPawnCombatComponent* AArcaneEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent.Get();
}

UPawnUIComponent* AArcaneEnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent.Get();
}

UEnemyUIComponent* AArcaneEnemyCharacter::GetEnemyUIComponent() const
{
	return EnemyUIComponent.Get();
}

AActor* AArcaneEnemyCharacter::GetCurrentTargetActor()
{
	if (AArcaneAIController* AIController = Cast<AArcaneAIController>(GetController()))
	{
		return AIController->GetCurrentTargetActor();
	}
	
	return nullptr;
}

void AArcaneEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UArcaneWidgetBase* HealthWidget = Cast<UArcaneWidgetBase>(EnemyHealthWidgetComponent->GetUserWidgetObject()))
	{
		HealthWidget->InitEnemyCreatedWidget(this);
	}
	
}

void AArcaneEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitEnemyStartupData();
}

void AArcaneEnemyCharacter::DeathMontageFinished()
{
	Super::DeathMontageFinished();
	
	BP_OnEnemyDeath();

}

void AArcaneEnemyCharacter::InitEnemyStartupData()
{
	int32 AbilityApplyLevel = 1;
	if (AArcaneGameModeBase* ArcaneGameMode = GetWorld()->GetAuthGameMode<AArcaneGameModeBase>())
	{
		switch (ArcaneGameMode->GetGameDifficulty())
		{
		case EArcaneGameDifficulty::Easy:
			AbilityApplyLevel = 1;
			break;
		case EArcaneGameDifficulty::Normal:
			AbilityApplyLevel = 2;
			break;
		case EArcaneGameDifficulty::Hard:
			AbilityApplyLevel = 3;
			break;
		case EArcaneGameDifficulty::Insane:
			AbilityApplyLevel = 4;
			break;
		case EArcaneGameDifficulty::None:
			AbilityApplyLevel = 1;
			break;
		}
	}
	
	// 对于TSoftObjectPtr类型的数据，需要先加载数据，然后再使用
	// Enemy角色的生成，我们希望他是异步生成的
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartupData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this, AbilityApplyLevel]()
		{
			if (UDataAsset_StartupDadaBase* LoadedData = CharacterStartupData.Get())
			{
				LoadedData->GiveToAbilitySystemComponent(ArcaneAbilitySystemComponent.Get(), AbilityApplyLevel);
			}
		})
	);
}
