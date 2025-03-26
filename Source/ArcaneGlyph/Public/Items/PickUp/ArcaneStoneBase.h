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

protected:
	virtual void OnPickUpCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
