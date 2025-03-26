// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArcanePickUpBase.generated.h"

class UWidgetComponent;
class USphereComponent;

UCLASS()
class ARCANEGLYPH_API AArcanePickUpBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArcanePickUpBase();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pick Up Interaction")
	TObjectPtr<USphereComponent> PickUpCollisionSphere;	// 拾取碰撞球体

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> PickUpWidgetComponent;	// 拾取小部件组件

	UFUNCTION()
	virtual void OnPickUpCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	// 拾取碰撞开始重叠

	UFUNCTION()
	virtual void OnPickUpCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);	// 拾取碰撞结束重叠

	
};
