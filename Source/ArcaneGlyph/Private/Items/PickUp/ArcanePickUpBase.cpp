// INVI_1998 All Rights Reserved.


#include "Items/PickUp/ArcanePickUpBase.h"

#include "Components/SphereComponent.h"


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
}

void AArcanePickUpBase::OnPickUpCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}


