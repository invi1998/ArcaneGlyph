// INVI_1998 All Rights Reserved.


#include "Items/PickUp/ArcanePickUpBase.h"

#include "ArcaneDebugHelper.h"
#include "ArcaneGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/ArcaneAbilitySystemComponent.h"
#include "Characters/ArcaneHeroCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"


// Sets default values
AArcanePickUpBase::AArcanePickUpBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickUpCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpCollisionSphere"));
	SetRootComponent(PickUpCollisionSphere);
	PickUpCollisionSphere->InitSphereRadius(50.f);
	PickUpCollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickUpCollisionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &AArcanePickUpBase::OnPickUpCollisionBeginOverlap);
	PickUpCollisionSphere->OnComponentEndOverlap.AddUniqueDynamic(this, &AArcanePickUpBase::OnPickUpCollisionEndOverlap);
	
	PickUpWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidgetComponent"));
	PickUpWidgetComponent->SetupAttachment(GetRootComponent());
	PickUpWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	
}

void AArcanePickUpBase::Consume(UArcaneAbilitySystemComponent* AbilitySystemComponent, int32 InApplyLevel)
{
}

void AArcanePickUpBase::BeginPlay()
{
	Super::BeginPlay();

	PickUpWidgetComponent->SetVisibility(false);
	
}

void AArcanePickUpBase::OnPickUpCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AArcaneHeroCharacter* HeroCharacter = Cast<AArcaneHeroCharacter>(OtherActor))
	{
		// 显示拾取UI
		PickUpWidgetComponent->SetVisibility(true);
		
		ULocalPlayer* LocalPlayer = HeroCharacter->GetController<APlayerController>()->GetLocalPlayer();
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			// 确保当前没有重复添加输入映射上下文
			if (!EnhancedInputSubsystem->HasMappingContext(PickUpInputMappingContext))
			{
				EnhancedInputSubsystem->AddMappingContext(PickUpInputMappingContext, MappingPriority);
			}
		}
	}
}

void AArcanePickUpBase::OnPickUpCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AArcaneHeroCharacter* HeroCharacter = Cast<AArcaneHeroCharacter>(OtherActor))
	{
		// 隐藏拾取UI
		PickUpWidgetComponent->SetVisibility(false);
		
		ULocalPlayer* LocalPlayer = HeroCharacter->GetController<APlayerController>()->GetLocalPlayer();
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			// 移除输入映射上下文
			EnhancedInputSubsystem->RemoveMappingContext(PickUpInputMappingContext);
		}
	}
}


