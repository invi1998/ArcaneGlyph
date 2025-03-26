// INVI_1998 All Rights Reserved.


#include "Items/PickUp/ArcanePickUpBase.h"

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

void AArcanePickUpBase::BeginPlay()
{
	Super::BeginPlay();

	PickUpWidgetComponent->SetVisibility(false);
	
}

void AArcanePickUpBase::OnPickUpCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 显示拾取UI
	PickUpWidgetComponent->SetVisibility(true);
}

void AArcanePickUpBase::OnPickUpCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 隐藏拾取UI
	PickUpWidgetComponent->SetVisibility(false);
}


