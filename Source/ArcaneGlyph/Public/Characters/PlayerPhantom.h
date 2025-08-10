// INVI_1998 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerPhantom.generated.h"

class UPoseableMeshComponent;
class AArcaneHeroCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPhantomDestroyedDelegate);

UCLASS()
class ARCANEGLYPH_API APlayerPhantom : public AActor, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	friend class UArcaneHeroStealthAbility; // 允许 UArcaneHeroStealthAbility 访问私有成员

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

	static FPhantomDestroyedDelegate OnPhantomDestroyed; // 幻影销毁委托

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
	UPROPERTY(EditDefaultsOnly)
	UMaterialInstanceDynamic* PhantomMaterial;
    
	// 淡出曲线
	UPROPERTY(EditDefaultsOnly, Category = "Stealth|Effects")
	UCurveFloat* FadeCurve;
	
};
