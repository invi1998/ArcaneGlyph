// INVI_1998 All Rights Reserved.


#include "Characters/ArcaneEnemyCharacter.h"

#include "Component/Combat/EnemyCombatComponent.h"
#include "Component/UI/EnemyUIComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "DataAssets/StartupData/DataAsset_EnemyStartupDada.h"
#include "Engine/AssetManager.h"
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

	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollisionBox"));
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AArcaneEnemyCharacter::OnBodyCollisionBoxBeginOverlap);

	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionBox"));
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AArcaneEnemyCharacter::OnBodyCollisionBoxBeginOverlap);

	HeadCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadCollisionBox"));
	HeadCollisionBox->SetupAttachment(GetMesh());
	HeadCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeadCollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	HeadCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AArcaneEnemyCharacter::OnBodyCollisionBoxBeginOverlap);
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

#if WITH_EDITOR
void AArcaneEnemyCharacter::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 如果修改了左手碰撞盒的骨骼名称，那么需要重新将碰撞盒附加到新的骨骼上
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(AArcaneEnemyCharacter, LeftHandCollisionBoxAttachBoneName))
	{
		LeftHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftHandCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(AArcaneEnemyCharacter, RightHandCollisionBoxAttachBoneName))
	{
		RightHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightHandCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(AArcaneEnemyCharacter, HeadCollisionBoxAttachBoneName))
	{
		HeadCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, HeadCollisionBoxAttachBoneName);
	}
}
#endif

void AArcaneEnemyCharacter::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
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
