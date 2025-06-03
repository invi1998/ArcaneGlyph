// INVI_1998 All Rights Reserved.


#include "Game/ArcaneSurvialGameModeBase.h"

void AArcaneSurvialGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AArcaneSurvialGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AArcaneSurvialGameModeBase::SetCurrentSurvialState(EArcaneSurvialGameModeState InState)
{
	CurrentSurvialGameModeState = InState;

	OnSurvialGameModeStateChanged.Broadcast(CurrentSurvialGameModeState);
}
