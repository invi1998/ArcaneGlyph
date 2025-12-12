// INVI_1998 All Rights Reserved.


#include "Characters/ArcaneCharacterBase.h"

#include "ArcaneBlueprintFunctionLibrary.h"
#include "ArcaneDebugHelper.h"
#include "ArcaneGameplayTags.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "AbilitySystem/ArcaneAttributeSet.h"
#include "MotionWarpingComponent.h"
#include "Component/Combat/PawnCombatComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AArcaneCharacterBase::AArcaneCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;		// 角色模型不接受贴花

	ArcaneAbilitySystemComponent = CreateDefaultSubobject<UArcaneAbilitySystemComponent>(TEXT("ArcaneAbilitySystemComponent"));
	ArcaneAttributeSet = CreateDefaultSubobject<UArcaneAttributeSet>(TEXT("ArcaneAttributeSet"));

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	
	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollisionBox"));
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AArcaneCharacterBase::OnBodyCollisionBoxBeginOverlap);

	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionBox"));
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AArcaneCharacterBase::OnBodyCollisionBoxBeginOverlap);

	HeadCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadCollisionBox"));
	HeadCollisionBox->SetupAttachment(GetMesh());
	HeadCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	HeadCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AArcaneCharacterBase::OnBodyCollisionBoxBeginOverlap);

	LeftFootCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFootCollisionBox"));
	LeftFootCollisionBox->SetupAttachment(GetMesh());
	LeftFootCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftFootCollisionBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	LeftFootCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AArcaneCharacterBase::OnBodyCollisionBoxBeginOverlap);

	RightFootCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFootCollisionBox"));
	RightFootCollisionBox->SetupAttachment(GetMesh());
	RightFootCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightFootCollisionBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	RightFootCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AArcaneCharacterBase::OnBodyCollisionBoxBeginOverlap);

	BindGASChangedDelegate();
	
}

UAbilitySystemComponent* AArcaneCharacterBase::GetAbilitySystemComponent() const
{
	// return GetAbilitySystemComponent();
	return GetArcaneAbilitySystemComponent();
}

UPawnCombatComponent* AArcaneCharacterBase::GetPawnCombatComponent() const
{
	return nullptr;
}

UPawnUIComponent* AArcaneCharacterBase::GetPawnUIComponent() const
{
	return nullptr;
}

bool AArcaneCharacterBase::IsCharacterAlive() const
{
	if (ArcaneAttributeSet)
	{
		return ArcaneAttributeSet->GetCurrentHealth() > 0.0f;
	}
	return false;
}

bool AArcaneCharacterBase::IsAlive()
{
	return UArcaneBlueprintFunctionLibrary::DoseActorHasGameplayTag(this, ArcaneGameplayTags::Shared_Status_Dead) == false;
}

void AArcaneCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (ArcaneAbilitySystemComponent)
	{
		ArcaneAbilitySystemComponent->InitAbilityActorInfo(this, this);

		// 确保角色启动数据有效，ensure 会在 Debug 编译模式下检查条件是否为真，如果为假则会触发断言
		// ensure(!CharacterStartupData.IsNull());
		ensureMsgf(!CharacterStartupData.IsNull(), TEXT("%s CharacterStartupData is null!"), *GetName());
	}
}

void AArcaneCharacterBase::StartDeathSequence()
{
	OnDeath();
		
	// 取消所有能力
	if (ArcaneAbilitySystemComponent)
	{
		ArcaneAbilitySystemComponent->CancelAllAbilities();
	}
	
	PlayDeathAnimation();
		
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AArcaneCharacterBase::PlayDeathAnimation()
{
	if (DeathMontage && GetMesh())
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			// 2. 停止所有蒙太奇（可选）
			AnimInstance->StopAllMontages(0.0f);
			
			float MontageDuration = GetMesh()->GetAnimInstance()->Montage_Play(
				DeathMontage,   // 蒙太奇资源
				1.0f,           // 播放速率
				EMontagePlayReturnType::MontageLength, // 返回类型
				0.0f,           // 开始时间
				true           // 停止所有其他蒙太奇
			);
			
			GetWorld()->GetTimerManager().ClearTimer(DeathMontageTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(DeathMontageTimerHandle, this, &AArcaneCharacterBase::DeathMontageFinished, MontageDuration + DeathMontageFinishTimeOffset, false);
	
		}
	}
}

void AArcaneCharacterBase::OnDeath()
{
}

void AArcaneCharacterBase::OnDeadTagChanged(FGameplayTag InGameplayTag, int Count)
{
	if (Count != 0)
	{
		StartDeathSequence();
	}
	else
	{
		// Respawn();
	}
}

void AArcaneCharacterBase::BindGASChangedDelegate()
{
	if (ArcaneAbilitySystemComponent)
	{
		ArcaneAbilitySystemComponent->RegisterGameplayTagEvent(ArcaneGameplayTags::Shared_Status_Dead, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AArcaneCharacterBase::OnDeadTagChanged);
	}
}

void AArcaneCharacterBase::DeathMontageFinished()
{
	
}

void AArcaneCharacterBase::SetRagdollPhysics(bool bEnabled)
{
}

#if WITH_EDITOR
void AArcaneCharacterBase::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 如果修改了左手碰撞盒的骨骼名称，那么需要重新将碰撞盒附加到新的骨骼上
	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(AArcaneCharacterBase, LeftHandCollisionBoxAttachBoneName))
   	{
   		LeftHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftHandCollisionBoxAttachBoneName);
   	}
   
   	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(AArcaneCharacterBase, RightHandCollisionBoxAttachBoneName))
   	{
   		RightHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightHandCollisionBoxAttachBoneName);
   	}
   
   	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(AArcaneCharacterBase, HeadCollisionBoxAttachBoneName))
   	{
   		HeadCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, HeadCollisionBoxAttachBoneName);
   	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(AArcaneCharacterBase, LeftFootCollisionBoxAttachBoneName))
   	{
   		LeftFootCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftFootCollisionBoxAttachBoneName);
   	}
   
   	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(AArcaneCharacterBase, RightFootCollisionBoxAttachBoneName))
   	{
   		RightFootCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightFootCollisionBoxAttachBoneName);
   	}
    	
}
#endif

void AArcaneCharacterBase::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		// 如果目标是敌对的
		if (UArcaneBlueprintFunctionLibrary::IsTargetPawnHostile(this, HitPawn))
		{
			GetPawnCombatComponent()->OnHitTargetActor(HitPawn, 1);
		}
	}
}




