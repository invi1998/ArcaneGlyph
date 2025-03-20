// INVI_1998 All Rights Reserved.


#include "Items/PickUp/ArcaneStoneBase.h"


// Sets default values
AArcaneStoneBase::AArcaneStoneBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AArcaneStoneBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArcaneStoneBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

