// INVI_1998 All Rights Reserved.


#include "Characters/PlayerPhantom.h"

#include "K2Node_AddComponent.h"
#include "Characters/ArcaneHeroCharacter.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
APlayerPhantom::APlayerPhantom()
{
	// 创建根组件
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootSceneComponent);

	// 创建PoseableMesh组件
	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	PoseableMesh->SetupAttachment(RootComponent);
	PoseableMesh->SetCollisionProfileName(TEXT("NoCollision"));
	PoseableMesh->SetGenerateOverlapEvents(false);	// 不生成重叠事件
	PoseableMesh->SetCastShadow(false);				// 不投射阴影
	
}

void APlayerPhantom::DestroyPhantom()
{
	Destroy();
}

void APlayerPhantom::InitializePhantom(AArcaneHeroCharacter* OriginalCharacter, float Duration)
{
	if (!OriginalCharacter)
	{
		return;
	}

	USkeletalMeshComponent* OriginalMesh = OriginalCharacter->GetMesh();
	if (!OriginalMesh || !OriginalMesh->GetSkeletalMeshAsset()) return;

	// 设置位置和旋转
	SetActorLocation(OriginalCharacter->GetActorLocation());
	SetActorRotation(OriginalCharacter->GetActorRotation());

	// 复制网格和材质
	PoseableMesh->SetSkinnedAssetAndUpdate(OriginalMesh->GetSkeletalMeshAsset());

	// 复制骨骼姿势
	PoseableMesh->CopyPoseFromSkeletalComponent(OriginalMesh);

	// 设置生命周期
	TotalLifetime = Duration;
	CurrentLifetime = 0.0f;
	bIsFading = true;

	// 设置定时销毁
	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimer,
		this,
		&APlayerPhantom::DestroyPhantom,
		TotalLifetime,
		false // 不循环
	);
	
}

void APlayerPhantom::BeginPlay()
{
	Super::BeginPlay();

	// 设置淡出材质
	if (PoseableMesh->GetMaterials().Num() > 0)
	{
		UMaterialInterface* BaseMaterial = PoseableMesh->GetMaterial(0);
		if (BaseMaterial)
		{
			PhantomMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
			PoseableMesh->SetMaterial(0, PhantomMaterial);
            
			// 设置初始透明度
			if (PhantomMaterial)
			{
				PhantomMaterial->SetScalarParameterValue("Opacity", 1.0f);
			}
		}
	}
}

void APlayerPhantom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 更新淡出效果
	if (bIsFading && PhantomMaterial)
	{
		CurrentLifetime += DeltaTime;
		float FadeProgress = FMath::Clamp(CurrentLifetime / TotalLifetime, 0.0f, 1.0f);
        
		// 使用曲线控制淡出效果
		float FadeValue = FadeCurve ? FadeCurve->GetFloatValue(FadeProgress) : 1.0f - FadeProgress;
		PhantomMaterial->SetScalarParameterValue("Opacity", FadeValue);
	}
}


