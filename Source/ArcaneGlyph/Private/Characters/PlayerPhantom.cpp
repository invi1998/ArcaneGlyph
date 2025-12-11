// INVI_1998 All Rights Reserved.


#include "Characters/PlayerPhantom.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneGameplayTags.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/ArcaneHeroStealthAbility.h"
#include "Characters/ArcaneHeroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Controllers/ArcaneAIController.h"
#include "Controllers/ArcaneHeroController.h"
#include "Engine/OverlapResult.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"

// Sets default values
APlayerPhantom::APlayerPhantom()
{
	// 创建根组件
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootSceneComponent);

	// 创建PoseableMesh组件
	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	PoseableMesh->SetupAttachment(RootComponent);
	PoseableMesh->SetCollisionProfileName(TEXT("NoCollision"));
	PoseableMesh->SetGenerateOverlapEvents(false);	// 不生成重叠事件
	PoseableMesh->SetCastShadow(true);				// 不投射阴影
	
	// 创建刺激源组件
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimulusSource"));
	StimulusSource->RegisterForSense(UAISense_Sight::StaticClass());
	StimulusSource->RegisterForSense(UAISense_Hearing::StaticClass());
	StimulusSource->RegisterWithPerceptionSystem();
	
}

void APlayerPhantom::DestroyPhantom()
{
	if (AArcaneHeroController* HeroController = CachedOriginalCharacter ? Cast<AArcaneHeroController>(CachedOriginalCharacter->GetController()) : nullptr)
	{
		HeroController->SetGenericTeamId(FGenericTeamId(0)); // 恢复玩家的团队 ID 为 0
	}
	
	// 恢复玩家材质
	if (CachedOriginalCharacter)
	{
		if (USkeletalMeshComponent* OriginalMesh = CachedOriginalCharacter->GetMesh())
		{
			OriginalMesh->SetColorParameterValueOnMaterials(FName("Light1"), FLinearColor::Black);
			OriginalMesh->SetColorParameterValueOnMaterials(FName("Light2"), FLinearColor::Black);
			
		}
		// 移除Player.Status.StealthPhantom标签
		UArcaneBlueprintFunctionLibrary::RemoveGameplayTagFromActorIfHas(
			CachedOriginalCharacter,
			ArcaneGameplayTags::Player_Status_StenlthPhantom
		);
	}
	
	OnPhantomDestroyed.Broadcast();
	// 清理定时器
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(DestroyTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(DestroyTimer);
	}
	
	Destroy();
}

void APlayerPhantom::InitializePhantom(AArcaneHeroCharacter* OriginalCharacter, float Duration)
{
	if (!OriginalCharacter)
	{
		return;
	}
	
	CachedOriginalCharacter = OriginalCharacter;

	USkeletalMeshComponent* OriginalMesh = OriginalCharacter->GetMesh();
	if (!OriginalMesh || !OriginalMesh->GetSkeletalMeshAsset()) return;

	// 设置位置和旋转
	SetActorTransform(OriginalCharacter->GetActorTransform());
	
	// 同时，因为角色骨骼旋转初始时yaw做了-90度旋转补偿，所以幻影也需要做相同的补偿
	FRotator AdjustedRotation = GetActorRotation();
	AdjustedRotation.Yaw -= 90.0f;
	SetActorRotation(AdjustedRotation);
	
	// 1. 首先获取角色的胶囊体组件
	UCapsuleComponent* CharacterCapsule = OriginalCharacter->FindComponentByClass<UCapsuleComponent>();
	USkeletalMeshComponent* CharacterMesh = OriginalCharacter->GetMesh();
    
	if (!CharacterMesh) return;
    
	// 2. 计算正确的位置
	FVector TargetLocation = OriginalCharacter->GetActorLocation();
    
	// 如果有胶囊体，使用胶囊体底部
	if (CharacterCapsule)
	{
		float CapsuleHalfHeight = CharacterCapsule->GetScaledCapsuleHalfHeight();
		TargetLocation.Z -= CapsuleHalfHeight; // 移动到胶囊体底部
	}
    
	// 3. 检查地面位置
	FHitResult GroundHit;
	FVector TraceStart = OriginalCharacter->GetActorLocation();
	FVector TraceEnd = TraceStart - FVector(0, 0, 200.0f); // 向下检测200单位
    
	if (GetWorld()->LineTraceSingleByChannel(
		GroundHit,
		TraceStart,
		TraceEnd,
		ECC_WorldStatic,
		FCollisionQueryParams(SCENE_QUERY_STAT(PhantomGroundTrace), false, OriginalCharacter)))
	{
		// 使用检测到的地面位置
		TargetLocation.Z = GroundHit.Location.Z;
	}
    
	// 4. 设置位置
	SetActorLocation(TargetLocation);

	// 复制网格和材质
	PoseableMesh->SetSkinnedAssetAndUpdate(OriginalMesh->GetSkeletalMeshAsset());

	// 复制骨骼姿势
	PoseableMesh->CopyPoseFromSkeletalComponent(OriginalMesh);
	
	// 同时，以防玩家角色本身存在缩放，幻影也需要复制该缩放
	SetActorScale3D(OriginalCharacter->GetActorScale3D());

	// 设置生命周期
	TotalLifetime = Duration;
	CurrentLifetime = 0.0f;
	bIsFading = true;

	// 设置定时销毁
	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimer,
		this,
		&APlayerPhantom::DestroyPhantom,
		TotalLifetime,
		false // 不循环
	);

	// 同时幻影需要监听 UArcaneHeroStealthAbility::OnMeleeAttack
	if (UArcaneAbilitySystemComponent* AbilitySystemComponent = OriginalCharacter->GetArcaneAbilitySystemComponent())
	{
		// 监听 Shared_Event_MeleeAttack_1 事件，即当玩家在隐身状态时，如果发动攻击并且攻击命中AI，则结束当前隐身状态
		FGameplayTagContainer MeleeAttackEventTags;
		MeleeAttackEventTags.AddTag(ArcaneGameplayTags::Shared_Event_MeleeAttack_1);
		OnMeleeAttackEventDelegateHandle = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(
			MeleeAttackEventTags,
			FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &APlayerPhantom::OnReceiveMeleeAttackEvent)
		);
	}
	
	NotifyNearbyAI();
	
}

void APlayerPhantom::BeginPlay()
{
	Super::BeginPlay();

	// 设置淡出材质
	if (PoseableMesh->GetMaterials().Num() > 0)
	{
		UMaterialInterface* BaseMaterial = PoseableMesh->GetMaterial(0);
		if (BaseMaterial)
		{
			PhantomMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
			PoseableMesh->SetMaterial(0, PhantomMaterial);
            
			// 设置初始透明度
			if (PhantomMaterial)
			{
				PhantomMaterial->SetScalarParameterValue("Opacity", 1.0f);
			}
		}
	}
}

void APlayerPhantom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 更新淡出效果
	if (bIsFading && PhantomMaterial)
	{
		CurrentLifetime += DeltaTime;
		float FadeProgress = FMath::Clamp(CurrentLifetime / TotalLifetime, 0.0f, 1.0f);
        
		// 使用曲线控制淡出效果
		float FadeValue = FadeCurve ? FadeCurve->GetFloatValue(FadeProgress) : 1.0f - FadeProgress;
		PhantomMaterial->SetScalarParameterValue("Opacity", FadeValue);
	}
}

void APlayerPhantom::NotifyNearbyAI()
{
	// 使用球体检测附近的AI控制器
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
    
	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECC_WorldStatic, // 或自定义通道
		FCollisionShape::MakeSphere(NotificationRadius),
		QueryParams
	);
    
	if (bHit)
	{
		for (const FOverlapResult& Result : OverlapResults)
		{
			if (AArcaneCharacterBase* EnemyCharacter = Cast<AArcaneCharacterBase>(Result.GetActor()))
			{
				if (AArcaneAIController* AIController = Cast<AArcaneAIController>(EnemyCharacter->GetController()))
				{
					// 直接调用AI控制器的方法
					AIController->UpdateEnemyAIPerceptionComponent(this);
				}
				
			}
			
		}
	}
}

void APlayerPhantom::OnReceiveMeleeAttackEvent(FGameplayTag GameplayTag, const FGameplayEventData* GameplayEventData)
{
	if (OnMeleeAttackEventDelegateHandle.IsValid())
	{
		// 清理监听
		FGameplayTagContainer MeleeAttackEventTags;
		MeleeAttackEventTags.AddTag(ArcaneGameplayTags::Shared_Event_MeleeAttack_1);
		if (UArcaneAbilitySystemComponent* AbilitySystemComponent = CachedOriginalCharacter ? CachedOriginalCharacter->GetArcaneAbilitySystemComponent() : nullptr)
		{
			AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(MeleeAttackEventTags, OnMeleeAttackEventDelegateHandle);
		}
		OnMeleeAttackEventDelegateHandle.Reset();
	}
	DestroyPhantom();
}


