// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArcanePickUpBase.h"
#include "ArcaneStoneBase.generated.h"

UCLASS()
class ARCANEGLYPH_API AArcaneStoneBase : public AArcanePickUpBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArcaneStoneBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
