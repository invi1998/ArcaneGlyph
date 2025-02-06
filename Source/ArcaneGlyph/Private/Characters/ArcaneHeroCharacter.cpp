// INVI_1998 All Rights Reserved.


#include "Characters/ArcaneHeroCharacter.h"


AArcaneHeroCharacter::AArcaneHeroCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AArcaneHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArcaneHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AArcaneHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


