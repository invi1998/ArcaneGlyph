// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerPhantom.generated.h"

class UPoseableMeshComponent;
class AArcaneHeroCharacter;

UCLASS()
class ARCANEGLYPH_API APlayerPhantom : public AActor, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	APlayerPhantom();

	virtual FGenericTeamId GetGenericTeamId() const override 
	{
		return TeamID;
	}

	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override 
	{
		TeamID = InTeamID;
	}

	void DestroyPhantom();
	// 初始化玩家幻影

	// OriginalCharacter: 原始角色，Duration: 幻影持续时间
	UFUNCTION(BlueprintCallable, Category = "Phantom")
	void InitializePhantom(AArcaneHeroCharacter* OriginalCharacter, float Duration);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UPoseableMeshComponent* PoseableMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootSceneComponent;

	FGenericTeamId TeamID = FGenericTeamId(0); // 默认为玩家团队(0)
    
	FTimerHandle DestroyTimer;
    
	// 淡出效果相关
	float CurrentLifetime = 0.0f;
	float TotalLifetime = 10.0f;
	bool bIsFading = false;
    
	// 材质实例用于淡出效果
	UPROPERTY()
	UMaterialInstanceDynamic* PhantomMaterial;
    
	// 淡出曲线
	UPROPERTY(EditDefaultsOnly, Category = "Stealth|Effects")
	UCurveFloat* FadeCurve;
	
};
